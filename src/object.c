#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "object.h"
#include <assert.h>

void Object_destroy(void *self)
{
    Object *obj = (Object *)self;
    
    if (!obj) return;
    
    // 释放描述字符串
    if (obj->description) {
        free(obj->description);
        obj->description = NULL;
				free(obj);
    }
    
    // 注意：这里不释放 obj 本身，由上层函数负责
}

void Object_describe(void *self)
{
	Object *obj = self;
	assert(obj != NULL);
	printf("%s.\n", obj->description);
}

int Object_init(void *self)
{
	//do nothing really
	return 1;
}

void *Object_move(void *self, int direction)
{
	printf("You can't go that direction.\n");
	return NULL;
}

int Object_attack(void *self, int damage)
{
	printf("You can't attack that.\n");
	return 0;
}

void *Object_new(size_t size, Object proto, char *description)
{
    // 设置默认函数
    if (!proto.init) proto.init = Object_init;
    if (!proto.describe) proto.describe = Object_describe;
    if (!proto.destroy) proto.destroy = Object_destroy;
    if (!proto.attack) proto.attack = Object_attack;
    if (!proto.move) proto.move = Object_move;

    // 分配内存
    Object *el = calloc(1, size);
    if (!el) {
        fprintf(stderr, "错误: 无法分配内存\n");
        return NULL;
    }

    // 复制原型
    *el = proto;

    // 复制描述
    if (description) {
        el->description = strdup(description);
        if (!el->description) {
            fprintf(stderr, "错误: 无法复制描述字符串\n");
            free(el);
            return NULL;
        }
    }

    // 初始化对象
    if (!el->init(el)) {
        fprintf(stderr, "错误: 对象初始化失败\n");
        if (el->description) free(el->description);
        free(el);
        return NULL;
    }

    return el;
}

