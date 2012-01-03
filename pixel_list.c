#include "pixel_list.h"

inline void plDestroy(DsegPixelList * list)
{
        DsegPixelList * tmp;

        while (list) {
                tmp = list->next;
                free(list);
                list = tmp; 
        }
}

inline void plInit(DsegPixelList * node, int x, int y, int label)
{
        node->next = NULL;
        node->pixel.x = x;
        node->pixel.y = y;
        node->pixel.label = label;
}

inline void plInsert(DsegPixelList * list, DsegPixelList * node)
{
        while (list->next)
                list = list->next;

        list->next = node;
}

inline int plDoesExist(DsegPixelList * list, int x, int y)
{
        /*
        while (list) {
                if (list->pixel.x == x && list->pixel.y == y)
                        return 1;

                list = head->next;
        }
        
        return 0;
        */
        return 0;
}

inline int plCount(DsegPixelList * list)
{
        int count = 0;
        
        while (list) {
                ++count;
                list = list->next;
        }

        return count;
}
