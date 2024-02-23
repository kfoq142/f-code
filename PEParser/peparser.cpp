//#include "stdafx.h"
#include <Windows.h>
#include <iostream>

int main(int argc, char* argv[]) {
	/*fgets(fileName, 255, stdin);*/
	//memcpy_s(&fileName, MAX_FILEPATH, argv[1], MAX_FILEPATH);
	HANDLE file = NULL;
	DWORD fileSize = NULL;
	DWORD bytesRead = NULL;
	LPVOID fileData = NULL;
	PIMAGE_DOS_HEADER dosHeader = {};
	PIMAGE_NT_HEADERS imageNTHeaders;
	PIMAGE_SECTION_HEADER sectionHeader = {};
	PIMAGE_SECTION_HEADER importSection = {};
	PIMAGE_SECTION_HEADER exportSection = {};
	IMAGE_IMPORT_DESCRIPTOR* importDescriptor = {};
	PIMAGE_EXPORT_DIRECTORY exportDirectory = {};
	PIMAGE_THUNK_DATA thunkData = {};
	DWORD thunk = NULL;
	DWORD rawOffset = NULL;
	DWORD numberofsection = NULL;

	if (argc != 2) {
		printf("==> Input: FileExecute.exe + Path File Parsing");
		return 0;
	}
	printf("Path PE File: %s\n\n", argv[1]);
	// open file
	/*file = CreateFile(L"D:\\Code_CTF\\a.exe", GENERIC_ALL, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);*/
	file = CreateFileA((LPCSTR)argv[1], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
	if (file == INVALID_HANDLE_VALUE) 
		printf("Can't open file");

	// allocate heap
	fileSize = GetFileSize(file, NULL);
	fileData = HeapAlloc(GetProcessHeap(), 0, fileSize);

	// read file bytes to memory
	ReadFile(file, fileData, fileSize, &bytesRead, NULL);

	// IMAGE_DOS_HEADER
	dosHeader = (PIMAGE_DOS_HEADER)fileData;
	printf("[1] ***** DOS HEADER *****\n");
	printf("\t0x%x\t\tMagic number (e_magic)\n", dosHeader->e_magic);
	printf("\t0x%x\t\tBytes on last page of file (e_eblp)\n", dosHeader->e_cblp);
	printf("\t0x%x\t\tPages in file (e_cp)\n", dosHeader->e_cp);
	printf("\t0x%x\t\tRelocations (e_crlc)\n", dosHeader->e_crlc);
	printf("\t0x%x\t\tSize of header in paragraphs (e_cparhdr)\n", dosHeader->e_cparhdr);
	printf("\t0x%x\t\tMinimum extra paragraphs needed (e_minalloc)\n", dosHeader->e_minalloc);
	printf("\t0x%x\t\tMaximum extra paragraphs needed (e_maxalloc)\n", dosHeader->e_maxalloc);
	printf("\t0x%x\t\tInitial (relative) SS value (e_ss)\n", dosHeader->e_ss);
	printf("\t0x%x\t\tInitial SP value (e_sp)\n", dosHeader->e_sp);
	printf("\t0x%x\t\tChecksum (e_csum)\n", dosHeader->e_csum);
	printf("\t0x%x\t\tInitial IP value (e_ip)\n", dosHeader->e_ip);
	printf("\t0x%x\t\tInitial (relative) CS value (e_cs)\n", dosHeader->e_cs);
	printf("\t0x%x\t\tFile address of relocation table (e_lfarlc)\n", dosHeader->e_lfarlc);
	printf("\t0x%x\t\tOverlay number (e_ovno)\n", dosHeader->e_ovno);
	printf("\t0x%x\t\tOEM identifier (e_oemid)\n", dosHeader->e_oemid);
	printf("\t0x%x\t\tOEM information (e_oeminfo) \n", dosHeader->e_oeminfo);
	printf("\t0x%x\t\tFile address of new exe header (e_lfanew)\n", dosHeader->e_lfanew);

	// IMAGE_NT_HEADERS
	imageNTHeaders = (PIMAGE_NT_HEADERS)((DWORD)fileData + (dosHeader->e_lfanew));
	printf("\n[2] ***** NT HEADERS *****\n");
	printf("\t[2.1] Signature (MZ)\n");
	printf("\t\t%x\t\tSignature\n", imageNTHeaders->Signature);

	// FILE_HEADER
	numberofsection = imageNTHeaders->FileHeader.NumberOfSections;
	printf("\n\t[2.1] FILE HEADER\n");
	printf("\t\t0x%x\t\tMachine\n", imageNTHeaders->FileHeader.Machine);
	printf("\t\t0x%x\t\tNumber of Sections\n", imageNTHeaders->FileHeader.NumberOfSections);
	printf("\t\t0x%x\tTime Stamp\n", imageNTHeaders->FileHeader.TimeDateStamp);
	printf("\t\t0x%x\t\tPointer to Symbol Table\n", imageNTHeaders->FileHeader.PointerToSymbolTable);
	printf("\t\t0x%x\t\tNumber of Symbols\n", imageNTHeaders->FileHeader.NumberOfSymbols);
	printf("\t\t0x%x\t\tSize of Optional Header\n", imageNTHeaders->FileHeader.SizeOfOptionalHeader);
	printf("\t\t0x%x\t\tCharacteristics\n", imageNTHeaders->FileHeader.Characteristics);

	// OPTIONAL_HEADER
	printf("\n\t[2.3] OPTIONAL HEADER\n");
	printf("\t\t0x%x\t\tMagic\n", imageNTHeaders->OptionalHeader.Magic);
	printf("\t\t0x%x\t\tMajor Linker Version\n", imageNTHeaders->OptionalHeader.MajorLinkerVersion);
	printf("\t\t0x%x\t\tMinor Linker Version\n", imageNTHeaders->OptionalHeader.MinorLinkerVersion);
	printf("\t\t0x%x\t\tSize Of Code\n", imageNTHeaders->OptionalHeader.SizeOfCode);
	printf("\t\t0x%x\t\tSize Of Initialized Data\n", imageNTHeaders->OptionalHeader.SizeOfInitializedData);
	printf("\t\t0x%x\t\tSize Of UnInitialized Data\n", imageNTHeaders->OptionalHeader.SizeOfUninitializedData);
	printf("\t\t0x%x\t\tAddress Of Entry Point (.text)\n", imageNTHeaders->OptionalHeader.AddressOfEntryPoint);
	printf("\t\t0x%x\t\tBase Of Code\n", imageNTHeaders->OptionalHeader.BaseOfCode);
	printf("\t\t0x%x\tImage Base\n", imageNTHeaders->OptionalHeader.ImageBase);
	printf("\t\t0x%x\t\tSection Alignment\n", imageNTHeaders->OptionalHeader.SectionAlignment);
	printf("\t\t0x%x\t\tFile Alignment\n", imageNTHeaders->OptionalHeader.FileAlignment);
	printf("\t\t0x%x\t\tMajor Operating System Version\n", imageNTHeaders->OptionalHeader.MajorOperatingSystemVersion);
	printf("\t\t0x%x\t\tMinor Operating System Version\n", imageNTHeaders->OptionalHeader.MinorOperatingSystemVersion);
	printf("\t\t0x%x\t\tMajor Image Version\n", imageNTHeaders->OptionalHeader.MajorImageVersion);
	printf("\t\t0x%x\t\tMinor Image Version\n", imageNTHeaders->OptionalHeader.MinorImageVersion);
	printf("\t\t0x%x\t\tMajor Subsystem Version\n", imageNTHeaders->OptionalHeader.MajorSubsystemVersion);
	printf("\t\t0x%x\t\tMinor Subsystem Version\n", imageNTHeaders->OptionalHeader.MinorSubsystemVersion);
	printf("\t\t0x%x\t\tWin32 Version Value\n", imageNTHeaders->OptionalHeader.Win32VersionValue);
	printf("\t\t0x%x\t\tSize Of Image\n", imageNTHeaders->OptionalHeader.SizeOfImage);
	printf("\t\t0x%x\t\tSize Of Headers\n", imageNTHeaders->OptionalHeader.SizeOfHeaders);
	printf("\t\t0x%x\t\tCheckSum\n", imageNTHeaders->OptionalHeader.CheckSum);
	printf("\t\t0x%x\t\tSubsystem\n", imageNTHeaders->OptionalHeader.Subsystem);
	printf("\t\t0x%x\t\tDllCharacteristics\n", imageNTHeaders->OptionalHeader.DllCharacteristics);
	printf("\t\t0x%x\t\tSize Of Stack Reserve\n", imageNTHeaders->OptionalHeader.SizeOfStackReserve);
	printf("\t\t0x%x\t\tSize Of Stack Commit\n", imageNTHeaders->OptionalHeader.SizeOfStackCommit);
	printf("\t\t0x%x\t\tSize Of Heap Reserve\n", imageNTHeaders->OptionalHeader.SizeOfHeapReserve);
	printf("\t\t0x%x\t\tSize Of Heap Commit\n", imageNTHeaders->OptionalHeader.SizeOfHeapCommit);
	printf("\t\t0x%x\t\tLoader Flags\n", imageNTHeaders->OptionalHeader.LoaderFlags);
	printf("\t\t0x%x\t\tNumber Of Rva And Sizes\n", imageNTHeaders->OptionalHeader.NumberOfRvaAndSizes);

	// DATA_DIRECTORIES
	printf("\n\t[2.3.1] DATA DIRECTORIES\n");
	printf("\t\tExport Directory Address: 0x%x; Size: 0x%x\n", imageNTHeaders->OptionalHeader.DataDirectory[0].VirtualAddress, imageNTHeaders->OptionalHeader.DataDirectory[0].Size);
	printf("\t\tImport Directory Address: 0x%x; Size: 0x%x\n", imageNTHeaders->OptionalHeader.DataDirectory[1].VirtualAddress, imageNTHeaders->OptionalHeader.DataDirectory[1].Size);
	// SECTION_HEADERS
	printf("\n[3] ***** SECTION HEADERS *****\n");
	printf("\tName\t\tVirtualSize\tVirtualAddress\tRawSize\t\tRawAddress\tRelocAddress\tLineNumbers\tRelocNumbers\tNumLineNum\tCharacteristics\n");
	// get offset to first section headeer
	DWORD sectionLocation = (DWORD)imageNTHeaders + sizeof(DWORD) + (DWORD)(sizeof(IMAGE_FILE_HEADER)) + (DWORD)imageNTHeaders->FileHeader.SizeOfOptionalHeader;
	DWORD sectionSize = (DWORD)sizeof(IMAGE_SECTION_HEADER);

	// get offset to the import directory RVA
	DWORD importDirectoryRVA = imageNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;

	// print section data
	for (int i = 0; i < imageNTHeaders->FileHeader.NumberOfSections; i++) {
		sectionHeader = (PIMAGE_SECTION_HEADER)sectionLocation;
		printf("\t%s", sectionHeader->Name);
		printf("\t\t0x%x", sectionHeader->Misc.VirtualSize);
		printf("\t\t0x%x", sectionHeader->VirtualAddress);
		printf("\t\t0x%x", sectionHeader->SizeOfRawData);
		printf("\t\t0x%x", sectionHeader->PointerToRawData);
		printf("\t\t0x%x", sectionHeader->PointerToRelocations);
		printf("\t\t0x%x", sectionHeader->PointerToLinenumbers);
		printf("\t\t0x%x", sectionHeader->NumberOfRelocations);
		printf("\t\t0x%x", sectionHeader->NumberOfLinenumbers);
		printf("\t\t0x%x\n", sectionHeader->Characteristics);

		// save section that contains import directory table
		if (importDirectoryRVA >= sectionHeader->VirtualAddress && importDirectoryRVA < sectionHeader->VirtualAddress + sectionHeader->Misc.VirtualSize) {
			importSection = sectionHeader;
			exportSection = sectionHeader;
		}
		sectionLocation += sectionSize;
	}

	printf("\n[5] ***** IMPORTS DIRECTORY *******\n");
	if (!(imageNTHeaders->FileHeader.Characteristics & IMAGE_FILE_DLL)) {
		// get file offset to import table
		rawOffset = (DWORD)fileData + importSection->PointerToRawData;

		// get pointer to import descriptor's file offset. Note that the formula for calculating file offset is: imageBaseAddress + pointerToRawDataOfTheSectionContainingRVAofInterest + (RVAofInterest - SectionContainingRVAofInterest.VirtualAddress)
		importDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)(rawOffset + (imageNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress - importSection->VirtualAddress));

		
		for (; importDescriptor->Name != 0; importDescriptor++) {
			// imported dll modules
			printf("\tDLL Name: %s\n", rawOffset + (importDescriptor->Name - importSection->VirtualAddress));
			thunk = importDescriptor->OriginalFirstThunk == 0 ? importDescriptor->FirstThunk : importDescriptor->OriginalFirstThunk;
			thunkData = (PIMAGE_THUNK_DATA)(rawOffset + (thunk - importSection->VirtualAddress));
			printf("\t\tFunctions:\n");
			// dll exported functions
			for (; thunkData->u1.AddressOfData != 0; thunkData++) {

				if (thunkData->u1.AddressOfData > 0x80000000) {

					printf("\t\tOrdinal: %x\n", (WORD)thunkData->u1.AddressOfData);
				}
				else {
					printf("\t\t\tName: %s\n", (rawOffset + (thunkData->u1.AddressOfData - importSection->VirtualAddress + 2)));
				}
			}
		}
	}
	else
		printf("====> No Import Directory\n");


	printf("\n[4] ***** EXPORTS DIRECTORY *****\n");
	if (imageNTHeaders->FileHeader.Characteristics & IMAGE_FILE_DLL) {
	
		char* fileName = strrchr(argv[1], '\\');
		HMODULE hModule;
		hModule = GetModuleHandle(++fileName);
		// IMAGE_DOS_HEADER
		dosHeader = (PIMAGE_DOS_HEADER)hModule;
		// IMAGE_NT_HEADERS
		imageNTHeaders = (PIMAGE_NT_HEADERS)((LPBYTE)hModule + (dosHeader->e_lfanew));

		exportDirectory = (PIMAGE_EXPORT_DIRECTORY)((LPBYTE)hModule + imageNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
		PDWORD Address = (PDWORD)((LPBYTE)hModule + exportDirectory->AddressOfFunctions);
		PDWORD Name = (PDWORD)((LPBYTE)hModule + exportDirectory->AddressOfNames);

		PWORD Ordinal = (PWORD)((LPBYTE)hModule + exportDirectory->AddressOfNameOrdinals);

		for (DWORD i = 0; i < exportDirectory->AddressOfFunctions; i++)
		{
			// if(!strcmp(FunctionName,(char*)dosHeader+Name[i]))
			// {
			printf("\tAddress: %x", (PVOID)((LPBYTE)hModule + Address[Ordinal[i]]));
			printf("\tName: %s\n", (char*)hModule + Name[i]);
			// }
		}
	}
	else
		printf("====> No Export Directory\n");
	return 0;
}