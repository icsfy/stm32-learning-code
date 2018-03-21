#include "fatfs_test_func.h"

/* Sample Code from http://elm-chan.org/fsw/ff/doc/readdir.html */
FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                printf("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }

    return res;
}

/* Example: http://elm-chan.org/fsw/ff/doc/stat.html */
void file_check(
  const TCHAR* path  /* [IN] Object name */
)
{
    FRESULT fr;
    FILINFO fno;

    printf("Test for '%s'...\n", path);

    fr = f_stat(path, &fno);
    switch (fr) {

    case FR_OK:
        printf("Size: %lu\n", fno.fsize);
        printf("Timestamp: %u/%02u/%02u, %02u:%02u\n",
               (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,
               fno.ftime >> 11, fno.ftime >> 5 & 63);
        printf("Attributes: %c%c%c%c%c\n",
               (fno.fattrib & AM_DIR) ? 'D' : '-',
               (fno.fattrib & AM_RDO) ? 'R' : '-',
               (fno.fattrib & AM_HID) ? 'H' : '-',
               (fno.fattrib & AM_SYS) ? 'S' : '-',
               (fno.fattrib & AM_ARC) ? 'A' : '-');
        break;

    case FR_NO_FILE:
        printf("It is not exist.\n");
        break;

    default:
        printf("An error occured. (%d)\n", fr);
    }
}

/* Example: http://elm-chan.org/fsw/ff/doc/getfree.html */
FRESULT get_space_info(
  const TCHAR* path,  /* [IN] Logical drive number */
  DWORD *fre_sect,
  DWORD *tot_sect
)
{
    FATFS *fs;
    FRESULT res;
    DWORD fre_clust;


    /* Get volume information and free clusters of drive 1 */
    res = f_getfree(path, &fre_clust, &fs);
    if (res) return res;

    /* Get total sectors and free sectors */
    *tot_sect = (fs->n_fatent - 2) * fs->csize;
    *fre_sect = fre_clust * fs->csize;

    /* Print the free space (assuming 4096 bytes/sector) */
    printf("%10lu KiB total drive space.\n%10lu KiB available.\n",
           *tot_sect / 2 * 8, *fre_sect / 2 * 8);
    return FR_OK;
}
