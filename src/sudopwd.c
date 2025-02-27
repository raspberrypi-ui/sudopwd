/*============================================================================
Copyright (c) 2025 Raspberry Pi
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
============================================================================*/

#include <gtk/gtk.h>
#include <locale.h>
#include <glib/gi18n.h>

/*----------------------------------------------------------------------------*/
/* Typedefs and macros                                                        */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Global data                                                                */
/*----------------------------------------------------------------------------*/

static GtkWidget *main_dlg, *pwd_entry;

/*----------------------------------------------------------------------------*/
/* Function prototypes                                                        */
/*----------------------------------------------------------------------------*/

static void handle_ok (GtkWidget *widget, gpointer data);
static void handle_cancel (GtkWidget *widget, gpointer data);
static gboolean close_prog (GtkWidget *widget, GdkEvent *event, gpointer data);

/*----------------------------------------------------------------------------*/
/* Function definitions                                                       */
/*----------------------------------------------------------------------------*/

static void handle_ok (GtkWidget *widget, gpointer data)
{
    printf ("%s\n", gtk_entry_get_text (GTK_ENTRY (pwd_entry)));
    gtk_main_quit ();
}

static void handle_cancel (GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();
}

static gboolean close_prog (GtkWidget *widget, GdkEvent *event, gpointer data)
{
    if (event->type == GDK_KEY_PRESS && ((GdkEventKey *) event)->keyval != GDK_KEY_Escape) return FALSE;

    gtk_main_quit ();
    return TRUE;
}

int main (int argc, char *argv[])
{
    GtkBuilder *builder;
    GtkWidget *wid;

    setlocale (LC_ALL, "");
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);

    gtk_init (&argc, &argv);

    gchar *objects[] = { "main_dlg", "cancel_btn", "ok_btn", "pwd_entry", NULL };
    builder = gtk_builder_new ();
    gtk_builder_add_objects_from_resource (builder, "/org/rpi/sudopwd/sudopwd.ui", objects, NULL);

    main_dlg = (GtkWidget *) gtk_builder_get_object (builder, "main_dlg");
    g_signal_connect (main_dlg, "delete_event", G_CALLBACK (close_prog), NULL);
    g_signal_connect (main_dlg, "key_press_event", G_CALLBACK (close_prog), NULL);

    wid = (GtkWidget *) gtk_builder_get_object (builder, "cancel_btn");
    g_signal_connect (wid, "clicked", G_CALLBACK (handle_cancel), NULL);

    wid = (GtkWidget *) gtk_builder_get_object (builder, "ok_btn");
    g_signal_connect (wid, "clicked", G_CALLBACK (handle_ok), NULL);

    pwd_entry = (GtkWidget *) gtk_builder_get_object (builder, "pwd_entry");
    g_signal_connect (pwd_entry, "activate", G_CALLBACK (handle_ok), NULL);

    g_object_unref (builder);

    gtk_widget_show (main_dlg);
    gtk_main ();

    return 0;
}

/* End of file */
/*----------------------------------------------------------------------------*/
