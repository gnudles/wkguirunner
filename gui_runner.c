#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <gtk/gtk.h>
#include <webkit2/webkit2.h>


struct GuiRunnerSettings
{
	int window_width;
	int window_height;
	const char* window_title;
	const char* html_uri;
	const char* extensions_dir;
};


static void
activate (GtkApplication* app,
          gpointer        user_data)
{


  GtkWidget *window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), ((struct GuiRunnerSettings*)user_data)->window_title );
  gtk_window_set_default_size (GTK_WINDOW (window), ((struct GuiRunnerSettings*)user_data)->window_width,
                                                    ((struct GuiRunnerSettings*)user_data)->window_height);
  WebKitWebView * web_view =  WEBKIT_WEB_VIEW( webkit_web_view_new() );
  //webkit_web_view_load_html (web_view, " <button onclick=\"sayHello()\">click me</button> ",  NULL);

  webkit_web_view_load_uri(web_view, ((struct GuiRunnerSettings*)user_data)->html_uri);



  gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET(web_view));


  gtk_widget_show_all (window);

}

static void
initialize_web_extensions (WebKitWebContext *context,
                           gpointer          user_data)
{
  if ( ((struct GuiRunnerSettings*)user_data)->extensions_dir != NULL )
  {
    webkit_web_context_set_web_extensions_directory ( context,
      ((struct GuiRunnerSettings*)user_data)->extensions_dir);
  }

}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;
  struct GuiRunnerSettings settings={.window_width=600,.window_height=600,.window_title="wkguirunner",.html_uri="https://webkitgtk.org", .extensions_dir=NULL};
  int c;
  opterr = 0;
  while ((c = getopt (argc, argv, "w:h:t:e:i:")) != -1)
  {
    switch (c)
    {
      case 'w':
        settings.window_width = strtol(optarg,0,10);
        break;
      case 'h':
        settings.window_height = strtol(optarg,0,10);
        break;
      case 't':
        settings.window_title = optarg;
        break;
      case 'e':
        settings.extensions_dir = optarg;
        break;
      case 'i':
        settings.html_uri = optarg;
        break;
      case '?':
        if (strchr ("whtei", optopt ) != NULL )
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt); 
        else fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
        return 1;
      default:
        abort ();
    }
  }

  app = gtk_application_new ("org.kapandaria.wkguirunner", G_APPLICATION_FLAGS_NONE);

  
  g_signal_connect (webkit_web_context_get_default (),
                   "initialize-web-extensions",
                    G_CALLBACK (initialize_web_extensions),
                    (gpointer)&settings);
  g_signal_connect (app, "activate", G_CALLBACK (activate), (gpointer)&settings);

  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}



