import socket
import os
import tldextract
import requests
from concurrent.futures import ThreadPoolExecutor
import utils

COMMON_PORTS = utils.load_common_ports()  # Load danh sách cổng phổ biến

def get_file(target):
    """
    Parse target input as either a file containing URLs or direct URL input.

    Args:
        target (str or list): Either a file path containing URLs or a direct URL/list

    Returns:
        list: List of URLs to scan
    """
    if isinstance(target, list):  
        return target
    elif os.path.isfile(target):  
        with open(target, "r", encoding="utf-8") as f:
            return [line.strip() for line in f if line.strip()]
    return [target]  # Nếu chỉ là một URL chuỗi

def service_scan(list_url, start_port, end_port, output, threads):
    """
    Scan for open ports and services on web targets.

    Args:
        list_url (str/list): URL(s) to scan or path to file containing URLs
        start_port (int): Starting port number
        end_port (int): Ending port number
        output (str): Path to output file (None for console output)
        threads (int): Number of concurrent scanning threads

    Output format:
        URL || Port || Service || Status
    """
    def open_socket(host, port, timeout=1):
        """
        Check if a specific port is open on a host.

        Args:
            host (str): Target hostname
            port (int): Port number to check
            timeout (int): Connection timeout in seconds

        Returns:
            tuple: (host, port, service, status) if port is open, None otherwise
        """
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
                s.settimeout(timeout)
                result = s.connect_ex((host, port))
                if result == 0:
                    service = COMMON_PORTS.get(port, "Unknown")  # Lấy tên dịch vụ
                    return host, port, service, "OPEN"
        except:
            pass
        return None

    def scan_ports(host, start_port, end_port, threads):
        """
        Scan a range of ports on a single host.

        Args:
            host (str): Target hostname
            start_port (int): Starting port number
            end_port (int): Ending port number
            threads (int): Number of concurrent threads

        Returns:
            list: List of tuples containing open port information
        """
        with ThreadPoolExecutor(max_workers=threads) as executor:
            results = executor.map(lambda p: open_socket(host, p), range(start_port, end_port + 1))
        return [res for res in results if res]

    def check_list(urls, start_port, end_port, threads, output):
        """
        Scan multiple URLs for open ports and services.

        Args:
            urls (list): List of URLs to scan
            start_port (int): Starting port number
            end_port (int): Ending port number
            threads (int): Number of concurrent threads
            output (str): Output file path
        """
        header = f"{'URL':<30} || {'Port':<6} || {'Service':<15} || {'Status'}\n" + "=" * 70
        output_lines = [header]

        for host in urls:
            open_ports = scan_ports(host, start_port, end_port, threads)
            if not open_ports:
                line = f"{host:<30} || {'-':<6} || {'-':<15} || {'CLOSED'}"
                output_lines.append(line)
            else:
                for url, port, service, status in open_ports:
                    line = f"{url:<30} || {port:<6} || {service:<15} || {status}"
                    output_lines.append(line)
        if output:
            with open(output, "w", encoding="utf-8") as f:
                f.write("\n".join(output_lines) + "\n")
        else:
            print("\n".join(output_lines))

    urls = get_file(list_url)
    check_list(urls, start_port, end_port, threads, output)

def subdomain_scan(target_file, output, threads):
    """
    Perform subdomain enumeration on target domains.

    Args:
        target_file (str): Target domain or file containing domains
        output (str): Path to output file (None for console output)
        threads (int): Number of concurrent scanning threads

    Output format:
        Subdomain || Status
    """
    def is_valid_subdomain(sub):
        if not sub or len(sub) > 255 or ".." in sub:
            return False
        return True

    def check_single_subdomain(sub):
        """
        Check if a subdomain exists and responds to HTTP requests.

        Args:
            sub (str): Subdomain to check

        Returns:
            str: Formatted result string if subdomain exists, None otherwise
        """
        if not is_valid_subdomain(sub):
            return None

        url = f"http://{sub}"
        try:
            response = requests.get(url, timeout=1)
            if response.status_code in [200, 301, 302]:  
                return f"{sub:<40} || EXISTS ({response.status_code})"
        except requests.RequestException:
            pass  # Không in ra subdomain không tồn tại

        return None

    def check_subdomains(subdomains, output, threads):
        """
        Check multiple subdomains for existence.

        Args:
            subdomains (list): List of subdomains to check
            output (str): Output file path
            threads (int): Number of concurrent threads
        """
        header = f"{'Subdomain':<40} || {'Status'}\n" + "=" * 60
        output_lines = [header]

        with ThreadPoolExecutor(max_workers=threads) as executor:
            results = list(executor.map(check_single_subdomain, subdomains))

        found = [res for res in results if res is not None]
        if found:
            output_lines.extend(found)
            if output:
                with open(output, "w", encoding="utf-8") as f:
                    f.write("\n".join(output_lines) + "\n")
            else:
                print("\n".join(output_lines))
        else:
            print("[❌] Không tìm thấy subdomain nào tồn tại.")

    def generate_subdomains(targets, subdomains):
        """
        Generate full subdomain names from base domains and subdomain list.

        Args:
            targets (list): List of base domains
            subdomains (list): List of subdomain prefixes

        Returns:
            list: List of complete subdomain names
        """
        full_subdomains = []
        for url in targets:
            extracted = tldextract.extract(url)
            main_domain = f"{extracted.domain}.{extracted.suffix}"
            if extracted.suffix and extracted.domain:  # Đảm bảo domain hợp lệ
                full_subdomains.extend([f"{sub}.{main_domain}" for sub in subdomains if is_valid_subdomain(f"{sub}.{main_domain}")])
        return full_subdomains

    def subdomain_scan(target_file, output, threads):
        urls = get_file(target_file)
        subdomains = utils.read_file_to_list(os.path.join(os.path.dirname(os.path.realpath(__file__)), r"Database\subdomain_list.txt"))
        full_domain = generate_subdomains(urls, subdomains)
        check_subdomains(full_domain, output, threads)
                
    subdomain_scan(target_file, output, threads)

def filepath_scan(target_file, output, threads):
    """
    Scan for existing file paths on web targets.

    Args:
        target_file (str): Target domain or file containing domains
        output (str): Path to output file (None for console output)
        threads (int): Number of concurrent scanning threads

    Output format:
        Domain || File Path || Status
    """
    def check_single_path(domain, path):
        """
        Check if a specific path exists on a domain.

        Args:
            domain (str): Target domain
            path (str): Path to check

        Returns:
            str: Formatted result string if path exists, None otherwise
        """
        full_url = f"https://{domain.rstrip('/')}/{path.lstrip('/')}"

        try:
            response = requests.get(full_url, timeout=5, allow_redirects=True)
            if response.status_code in [200, 301, 302, 403]:  # Chỉ chấp nhận nếu tồn tại
                return f"{domain:<30} || {full_url:<60} || EXISTS ✅ ({response.status_code})"
        except requests.RequestException:
            pass
        return None

    def check_paths(domains, paths, output, threads):
        """
        Check multiple paths across multiple domains.

        Args:
            domains (list): List of domains to check
            paths (list): List of paths to check
            output (str): Output file path
            threads (int): Number of concurrent threads
        """
        header = f"{'Domain':<30} || {'File Path':<60} || {'Status'}\n" + "=" * 130
        output_lines = [header]

        with ThreadPoolExecutor(max_workers=threads) as executor:
            results = executor.map(lambda args: check_single_path(*args), [(domain, path) for domain in domains for path in paths])
        
        found = [res for res in results if res is not None]
        
        if found:
            output_lines.extend(found)
            result_text = "\n".join(output_lines)
            if output:
                with open(output, "w", encoding="utf-8") as f:
                    f.write(result_text + "\n")  # Ghi vào file
                return
            print(result_text)  # In ra màn hình
        else:
            print("[❌] Không tìm thấy file nào tồn tại trên tất cả các domain.")

    def file_scan(target_file, output, threads):
        """Quét file path phía sau danh sách URL"""
        domains = get_file(target_file)  # Đọc danh sách domain từ file
        wordlist_path = os.path.join(os.path.dirname(__file__), "Database", "filepath_list.txt")
        wordlist = utils.read_file_to_list(wordlist_path)

        print("[🔍] Bắt đầu quét danh sách domain...\n")
        check_paths(domains, wordlist, output, threads)
    
    file_scan(target_file, output, threads)