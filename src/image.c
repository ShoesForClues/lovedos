/**
 * Copyright (c) 2017 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See LICENSE for details.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lib/dmt/dmt.h"
#include "lib/stb/stb_image.h"
#include "filesystem.h"
#include "image.h"
#include "palette.h"

int image_blendMode = IMAGE_NORMAL;
int image_flip = 0;
unsigned int image_color = 0x0f0f0f0f;


void image_setBlendMode(int mode) {
  image_blendMode = mode;
}

void image_setColor(pixel_t color) {
  image_color = color | (color << 8) | (color << 16) | (color << 24);
}

void image_setFlip(int mode) {
  image_flip = !!mode;
}



const char *image_init(image_t *self, const char *filename) {
  /* Loads an image file into the struct and inits the mask */
  const char *errmsg = NULL;
  void *filedata = NULL;
  unsigned char *data32 = NULL;
  memset(self, 0, sizeof(*self));

  /* Load file data */
  int size;
  filedata = filesystem_read(filename, &size);
  if (!filedata) {
    errmsg = "could not read file";
    goto fail;
  }

  /* Load 32bit image data */
  int width, height, n;
  data32 = stbi_load_from_memory(filedata, size, &width, &height, &n, 4);
  if (!data32) {
    errmsg = "could not load image file";
    goto fail;
  }

  /* Free file data */
  filesystem_free(filedata);
  filedata = NULL;

  /* Set dimensions and allocate memory */
  int sz = width * height;
  self->width = width;
  self->height = height;
  self->data = dmt_malloc(sz);

  /* Load pixels into struct, converting 32bit to 8bit paletted */
  int i;
  for (i = 0; i < width * height; i++) {
    unsigned char *p = data32 + i * 4;
    int r = p[0];
    int g = p[1];
    int b = p[2];
    int a = p[3];
    int idx = palette_colorToIdx(r, g, b);
    if (idx < 0) {
      errmsg = "color palette exhausted: use fewer unique colors";
      goto fail;
    }
    self->data[i] = (a >= 127) ? idx : 0;
  }

  /* Init mask */
  self->mask = dmt_malloc(sz);
  for (i = 0; i < sz; i++) {
    self->mask[i] = (self->data[i] == 0) ? 0xFF : 0x00;
  }

  /* Free 32bit pixel data, return NULL for no error */
  free(data32);
  data32 = NULL;

  return NULL;

fail:
  filesystem_free(filedata);
  free(data32);
  return errmsg;
}


void image_initBlank(image_t *self, int width, int height) {
  /* Creates a blank zeroset image with a zeroset mask. This function can be
   * used to init the image instead of image_init() */
  memset(self, 0, sizeof(*self));
  self->data = dmt_calloc(1, width * height);
  self->width = width;
  self->height = height;
  /* Init mask */
  self->mask = dmt_calloc(1, width * height);
}

void image_blit(image_t *self,pixel_t *buf,int bufw,int bufh,int dx,int dy,int dw,int dh,int sx,int sy,int sw,int sh) {
	int bitmap_width=self->width;
	int p;
	//int bitmap_height=self->height;
	for (int cy=0; cy<dh; cy++) {
		for (int cx=0; cx<dw; cx++) {
			if (self->mask[bitmap_width*(int)((cy/(double)dh*sh)+sy)+(int)((cx/(double)dw*sw)+sx)]==0) {
				p=self->data[bitmap_width*(int)((cy/(double)dh*sh)+sy)+(int)((cx/(double)dw*sw)+sx)];
				if (p==1) {
					buf[bufw*(cy+dy)+(cx+dx)]=image_color;
				} else {
					buf[bufw*(cy+dy)+(cx+dx)]=p;
				}
			}
		}
	}
}

void image_deinit(image_t *self) {
  dmt_free(self->data);
  dmt_free(self->mask);
}
