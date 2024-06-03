/*
 *  elf32 reader
 *  Created on: 2014-6
 *  Author: Chris.Z
 *  原文链接：https://blog.csdn.net/lifeshow/article/details/29198107
 */
#include <stdio.h>
#include <stdlib.h>
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#ifdef __x86_64
    #define Elf_Ehdr Elf64_Ehdr
    #define Elf_Shdr Elf64_Shdr
    #define Elf_Sym Elf64_Sym
    #define Elf_Rel Elf64_Rela
    #define ELF_R_SYM ELF64_R_SYM
    #define REL_DYN ".rela.dyn"
    #define REL_PLT ".rela.plt"
#else
    #define Elf_Ehdr Elf32_Ehdr
    #define Elf_Shdr Elf32_Shdr
    #define Elf_Sym Elf32_Sym
    #define Elf_Rel Elf32_Rel
    #define ELF_R_SYM ELF32_R_SYM
    #define REL_DYN ".rel.dyn"
    #define REL_PLT ".rel.plt"
#endif

#define LOG(...) printf(__VA_ARGS__);

/**
 * lookup the start address of a specific module(libc.so...) within current process
 * return 0 if FAILED
 */
static uint32_t get_module_base(pid_t pid, const char *module_path) {
	FILE *fp = NULL;
	char *pch = NULL;
	char filename[32];
	char line[512];
	uint32_t addr = 0;

	LOG("[+] get libc base...\n");
	if (pid < 0)
		snprintf(filename, sizeof(filename), "/proc/self/maps");
	else
		snprintf(filename, sizeof(filename), "/proc/%d/maps", pid);

	if ((fp = fopen(filename, "r")) == NULL) {
		LOG("[-]open %s failed!", filename);
		return 0;
	}

	while (fgets(line, sizeof(line), fp)) {
		if (strstr(line, module_path)) {
			pch = strtok(line, "-");
			addr = strtoul(pch, NULL, 16);
			break;
		}
	}

	fclose(fp);
	LOG("[+] libc base:0x%x...\n",addr);

	return addr;
}

/**
 * read the elf header
 * return 0 if SUCCESS
 */
static int read_header(int d, Elf_Ehdr **header)//read elf header structure
{
    *header = (Elf_Ehdr *)malloc(sizeof(Elf_Ehdr));

    if (lseek(d, 0, SEEK_SET) < 0)//seek to the begin of file
    {
        free(*header);

        return errno;
    }

    if (read(d, *header, sizeof(Elf_Ehdr)) <= 0)//read from begin,read sizof(Elf_Ehdr) bytes ==> header
    {
        free(*header);

        return errno = EINVAL;
    }

    return 0;
}

/**
 * read the section header
 * return 0 if SUCCESS
 */
static int read_section_table(int d, Elf_Ehdr const *header, Elf_Shdr **table)//read elf header,find section header base address
{
    size_t size;

    if (NULL == header)
        return EINVAL;

    size = header->e_shnum * sizeof(Elf_Shdr);//section numbers and total size
    *table = (Elf_Shdr *)malloc(size);

    if (lseek(d, header->e_shoff, SEEK_SET) < 0)//point to section header,offset 0
    {
        free(*table);

        return errno;
    }

    if (read(d, *table, size) <= 0)//read section header structure to **table
    {
        free(*table);

        return errno = EINVAL;
    }

    return 0;
}

/**
 * read the string section table
 * return 0 if SUCCESS
 */

static int read_string_table(int d, Elf_Shdr const *section, char const **strings)
{
    if (NULL == section)//section == > .dynstr section
        return EINVAL;

    *strings = (char const *)malloc(section->sh_size);

    if (lseek(d, section->sh_offset, SEEK_SET) < 0)
    {
        free((void *)*strings);

        return errno;
    }

    if (read(d, (char *)*strings, section->sh_size) <= 0)//strings include all strings in .dynstr sections
    {
        free((void *)*strings);

        return errno = EINVAL;
    }

    return 0;
}

int main()
{
    LOG("[+]Arm ELF32 reader...\n");
    uint32_t lic_base = get_module_base(-1,"/system/lib/libc.so");
    int descriptor = open("/system/lib/libc.so", O_RDONLY);//open libc.so,and return the handle
    Elf_Ehdr *header = NULL;//elf header
    Elf_Shdr *section_header = NULL;//section header array ptr
    char const *strings = NULL;//string table ptr
    read_header(descriptor,&header);
    LOG("[+]libc.so elf header:\n");
    LOG("[+]e_ident[EI_NIDENT]:   %s\n",header->e_ident);
    LOG("[+]e_type:%d(ET_DYN:%d,DYN (Shared object file))\n",header->e_type,ET_DYN);
    LOG("[+]e_machine:%d(EM_ARM:%d,Advanced RISC Machines)\n",header->e_machine,EM_ARM);
    LOG("[+]e_shoff:%d bytes\n",header->e_shoff);

    LOG("[+]libc.so section header:\n");
    read_section_table(descriptor,header,§ion_header);
    read_string_table(descriptor,§ion_header[header->e_shstrndx], &strings);//header->e_shstrndx ==>the index of string section header in section headers
    int i = 0;
    for(i = 0;i<header->e_shnum;++i)
    {
        LOG("Section[%d] name:%s,type:%d,addr:0x%x,offset:0x%x,size:%dbytes,etc...\n",i,&strings[section_header[i].sh_name],section_header[i].sh_type,section_header[i].sh_addr,section_header[i].sh_offset,section_header[i].sh_size);
    }
    close(descriptor);
    return 0;
}
