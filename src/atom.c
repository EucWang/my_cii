#include "atom.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "assert.h"
//#include <memory.h>
#include "mem.h"

/**
 * 计算数组指针的数组长度
 */
#define NELEMS(x) ((sizeof(x))/(sizeof((x)[0])))

static unsigned long scatter[] = {
2078917053, 143302914, 1027100827, 1953210302, 755253631, 2002600785,
1405390230, 45248011, 1099951567, 433832350, 2018585307, 438263339,
813528929, 1703199216, 618906479, 573714703, 766270699, 275680090,
1510320440, 1583583926, 1723401032, 1965443329, 1098183682, 1636505764,
980071615, 1011597961, 643279273, 1315461275, 157584038, 1069844923,
471560540, 89017443, 1213147837, 1498661368, 2042227746, 1968401469,
1353778505, 1300134328, 2013649480, 306246424, 1733966678, 1884751139,
744509763, 400011959, 1440466707, 1363416242, 973726663, 59253759,
1639096332, 336563455, 1642837685, 1215013716, 154523136, 593537720,
704035832, 1134594751, 1605135681, 1347315106, 302572379, 1762719719,
269676381, 774132919, 1851737163, 1482824219, 125310639, 1746481261,
1303742040, 1479089144, 899131941, 1169907872, 1785335569, 485614972,
907175364, 382361684, 885626931, 200158423, 1745777927, 1859353594,
259412182, 1237390611, 48433401, 1902249868, 304920680, 202956538,
348303940, 1008956512, 1337551289, 1953439621, 208787970, 1640123668,
1568675693, 478464352, 266772940, 1272929208, 1961288571, 392083579,
871926821, 1117546963, 1871172724, 1771058762, 139971187, 1509024645,
109190086, 1047146551, 1891386329, 994817018, 1247304975, 1489680608,
706686964, 1506717157, 579587572, 755120366, 1261483377, 884508252,
958076904, 1609787317, 1893464764, 148144545, 1415743291, 2102252735,
1788268214, 836935336, 433233439, 2055041154, 2109864544, 247038362,
299641085, 834307717, 1364585325, 23330161, 457882831, 1504556512,
1532354806, 567072918, 404219416, 1276257488, 1561889936, 1651524391,
618454448, 121093252, 1010757900, 1198042020, 876213618, 124757630,
2082550272, 1834290522, 1734544947, 1828531389, 1982435068, 1002804590,
1783300476, 1623219634, 1839739926, 69050267, 1530777140, 1802120822,
316088629, 1830418225, 488944891, 1680673954, 1853748387, 946827723,
1037746818, 1238619545, 1513900641, 1441966234, 367393385, 928306929,
946006977, 985847834, 1049400181, 1956764878, 36406206, 1925613800,
2081522508, 2118956479, 1612420674, 1668583807, 1800004220, 1447372094,
523904750, 1435821048, 923108080, 216161028, 1504871315, 306401572,
2018281851, 1820959944, 2136819798, 359743094, 1354150250, 1843084537,
1306570817, 244413420, 934220434, 672987810, 1686379655, 1301613820,
1601294739, 484902984, 139978006, 503211273, 294184214, 176384212,
281341425, 228223074, 147857043, 1893762099, 1896806882, 1947861263,
1193650546, 273227984, 1236198663, 2116758626, 489389012, 593586330,
275676551, 360187215, 267062626, 265012701, 719930310, 1621212876,
2108097238, 2026501127, 1865626297, 894834024, 552005290, 1404522304,
48964196, 5816381, 1889425288, 188942202, 509027654, 36125855,
365326415, 790369079, 264348929, 513183458, 536647531, 13672163,
313561074, 1730298077, 286900147, 1549759737, 1699573055, 776289160,
2143346068, 1975249606, 1136476375, 262925046, 92778659, 1856406685,
1884137923, 53392249, 1735424165, 1602280572
};

struct atom{
    struct atom * link; //一个链表
    int len;                
    char * str;             
} *buckets[2048];

int Atom_length(const char * str) {
    struct atom *p;
    int i;

    assert(str);
    for(i =0; i<NELEMS(buckets); i++) {
        for(p = buckets[i]; p ; p = p->link) {
            if(p->str == str) {
				return p->len;
            }
        }
    }
    assert(0);
    return 0;
}

const char * Atom_new(const char * str, int len) {
   printf("Atom_new() calling ... param1 is '%s', param2 is %d\n", str, len);
   unsigned long h;
   int i;
   struct atom *p;

   assert(str);
   assert(len >= 0);

   for(h = 0, i=0; i<len; i++) {
       h = (h<<1) + scatter[(unsigned char)str[i]];
   }

   h %= NELEMS(buckets);
   printf("scatter and hash, then h is %ld\n", h);

   for(p = buckets[h]; p; p = p->link) {
       if(len == p->len) {
           for(i =0; i<len && p->str[i]== str[i];) {
               i++;
           }
           if(i==len) {
               printf("find out from array, then return it.\n");
               return p->str;
           }
       }
   }

    printf("find nothing from array, then create one and add it to array.\n");
    //p = malloc(sizeof(*p) + len + 1);
	p = ALLOC(sizeof(*p) + len + 1);

    p->len = len;
    p->str = (char *)(p + 1);
    if(len > 0) {
        memcpy(p->str, str, len);
    }
    p->str[len] = '\0';
    p->link = buckets[h];
    buckets[h] = p;
    printf("index of buckets is %ld\n", h);

   return p->str;
}

const char * Atom_string(const char * str) {
    printf("Atom_string() calling.. , %s\n", str);
    assert(str);  //这里
    printf("Atom_string() running after assert()..\n");
    return Atom_new(str, strlen(str));
}

const char * Atom_int(long n) {
    char str[44];  //任意系统的long都不会超过43位数字, 多一位用来放\0
    memset(str, '\0', sizeof(str));

    // char *s = str + sizeof(str);  //指向数组末尾之后
    char *s = str+42;

    unsigned long m;  //处理n之后的值
    if(n == LONG_MIN) {  //如果n是最小的long值
        m = LONG_MAX + 1UL; //m为正最大+1, 这里处理补码情况,负数比正数多一个,所以+1
    } else if(n < 0) {    //如果n小于0
        m = -n;    //为正
    } else {
        m = n;    //本身的值
    }

    int length = 0;

    //将每一个数字转换成字符写入到数组中
    do {
        *s = m %10 + '0';
        s--;
        length++;
    } while((m/=10) > 0);

    if(n < 0) {  //是负数,则添加负号
        *s-- = '-';
        length++;
    }
    s++;

    printf("length is %d\n", length);
    // return Atom_new(s, (str + sizeof(str) - s));
    return Atom_new(s, length);
}


/**
 * 默认的比较函数
 * @param x
 * @param y
 * @return
 */
int cmpatom(const void * x, const void * y) {
    return x != y;
}

/**
 * 默认的hash函数
 * 原子是一个地址, 这个地址本省就可以用来作为hash码,
 * 右移两位是因为可能每个原子都起始于字边界(word boundary),因此最右侧两位可能是0
 * @param key
 * @return
 */
unsigned  hashatom(const void * key) {
    return (unsigned long) key>>2;
}



#define NHASH 29989
#define MULT 31

/**
 * 字符串比较函数
 * @param str1
 * @param str2
 * @return
 */
int str_cmp(const char * str1, const char * str2){
    return strcmp(str1, str2);
}

/**
 * 字符串的hash函数
 * @param str
 * @return
 */
int str_hash(const char * str) {
    unsigned int h = 0;
    const char * p = str;
    for (; *p; p++) {
        h = MULT * h + *p;
    }
    return h % NHASH;
}

/**
 * int型数据的比较函数
 * @param a int *
 * @param b  int *
 * @return  int
 */
int int_cmp(const int * a, const int * b) {
    if(*a > *b) {
        return 1;
    } else if(*a == *b) {
        return 0;
    } else {
        return -1;
    }
}

/**
 * int的哈希函数
 * @param num
 * @return
 */
int int_hash(const int * num) {
    return (MULT + *num) % NHASH;
}
