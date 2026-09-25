/********************************************************************
 *         This example code is from the book:
 *
 *           The X Window System: Programming and Applications with Xt
 *           Second OSF/Motif Edition
 *         by
 *           Douglas Young
 *           Prentice Hall, 1994
 *
 *         Copyright 1994 by Prentice Hall
 *         All Rights Reserved
 *
 *  Permission to use, copy, modify, and distribute this software for 
 *  any purpose except publication and without fee is hereby granted, provided 
 *  that the above copyright notice appear in all copies of the software.
 * *****************************************************************************/

/**************************************************
 * xpmlogo.c: Display the X Logo using Xpm format
 **************************************************/
#include <Xm/Xm.h>
#include <Xm/PushB.h>
#include <X11/xpm.h>  /* Non-standard header file */

#include "png.h"
#include "zlib.h"
#include "stdlib.h"

Widget CreateXlogoButton ( Widget parent );  

void readpng_version_info() {
    fprintf(stderr, "   Compiled with libpng %s; using libpng %s.\n",
      PNG_LIBPNG_VER_STRING, png_libpng_ver);
    fprintf(stderr, "   Compiled with zlib %s; using zlib %s.\n",
      ZLIB_VERSION, zlib_version);
}

Widget CreateXlogoButton(Widget parent)
{
    Widget button;
    Pixmap pix = None;
    Pixmap mask = None;
    Display *dpy = XtDisplay(parent);
    int status;

    XpmAttributes   attributes;

    button = XtCreateManagedWidget("button", xmPushButtonWidgetClass, parent, NULL, 0);

    Pixel bg_color;
    XtVaGetValues ( button,
                XmNdepth,    &attributes.depth,
                XmNcolormap, &attributes.colormap,
                XmNbackground, &bg_color,
                NULL);
    unsigned char bg_r = (bg_color >> 16) & 0xFF;
    unsigned char bg_g = (bg_color >> 8)  & 0xFF;
    unsigned char bg_b =  bg_color        & 0xFF;

    /*
     * Specify the visual to be used and set the XpmAttributes mask.
     */
    attributes.visual = DefaultVisual ( dpy, DefaultScreen ( dpy ) );
    attributes.valuemask = XpmDepth | XpmColormap | XpmVisual;

    // 2.Open PNG file
    FILE *fp = fopen("grand_ca.jpg.png", "rb");
    if (!fp) {
        fprintf(stderr, "Error opening file\n");
        return button;
    }

    // 3. initialize libpng
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) { fclose(fp); return button; }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) { png_destroy_read_struct(&png_ptr, NULL, NULL); fclose(fp); return button; }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        fclose(fp);
        return button;
    }

    png_init_io(png_ptr, fp);
    png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    int width = png_get_image_width(png_ptr, info_ptr);
    int height = png_get_image_height(png_ptr, info_ptr);
    png_bytep *row_pointers = png_get_rows(png_ptr, info_ptr);
    int channels = png_get_channels(png_ptr, info_ptr);

    // Set up XPM array
    int num_colors = width * height;
    int xpm_lines = 1 + num_colors + height;
    char **xpm_data = malloc(xpm_lines * sizeof(char *));


    // write header line
    xpm_data[0] = malloc(50);
    sprintf(xpm_data[0], "%d %d %d 5", width, height, num_colors);

    // create color palette and pixel array
    int color_index = 0;
    for (int y = 0; y < height; y++) {
        xpm_data[1 + num_colors + y] = malloc(width * 5 + 1);
        xpm_data[1 + num_colors + y][0] = '\0';

        for (int x = 0; x < width; x++) {
            png_bytep px = &(row_pointers[y][x * channels]);
            unsigned char r = px[0];
            unsigned char g = px[1];
            unsigned char b = px[2];

            // create char token for xpm
            char token[10];
            sprintf(token, "%c%c%c%c%c",
                    'a' + (color_index / 456976) % 26,
                    'a' + (color_index / 17576) % 26,
                    'a' + (color_index / 676) % 26,
                    'a' + (color_index / 26) % 26,
                    'a' + color_index % 26);

            xpm_data[1 + color_index] = malloc(50);

            // get alpha value (if available) and normalize 0.0..1,0
            float alpha = (channels == 4) ? (px[3] / 255.0f) : 1.0f;

            // alpha = 0 -> draw background
            // alpha = 1 -> draw pixel
            // alpha in between: mix in some transparency
            unsigned char final_r = (unsigned char)(r * alpha + bg_r * (1.0f - alpha));
            unsigned char final_g = (unsigned char)(g * alpha + bg_g * (1.0f - alpha));
            unsigned char final_b = (unsigned char)(b * alpha + bg_b * (1.0f - alpha));

            sprintf(xpm_data[1 + color_index], "%s c #%02X%02X%02X", token, final_r, final_g, final_b);

            // add token
            strcat(xpm_data[1 + num_colors + y], token);
            color_index++;
        }
    }

    // create Pixmap from data
    status = XpmCreatePixmapFromData(dpy, DefaultRootWindow ( dpy ),
                                     xpm_data, &pix, &mask, &attributes);

    // cleanup
    for (int i = 0; i < xpm_lines; i++) {
       free(xpm_data[i]);
    }
    free(xpm_data);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
    fclose(fp);

    // set pixmap in widget
    if (status == XpmSuccess && pix != None) {
        XtVaSetValues(button,
                      XmNlabelType, XmPIXMAP,
                      XmNlabelPixmap, pix,
                      NULL);

    } else {
        fprintf(stderr, "XPM-Fehler: Pixmap konnte nicht erstellt werden (%d).\n", status);
    }
    return button;
}
                            

void main ( int argc, char **argv )
{
    Widget       shell, button;
    XtAppContext app;

    readpng_version_info();

    shell = XtAppInitialize ( &app, "XPmlogo", NULL, 0,
                              &argc, argv, NULL, NULL, 0  );

    button = CreateXlogoButton( shell );

    XtRealizeWidget ( shell );
    XtAppMainLoop ( app );
}