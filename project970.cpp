#include <stdio.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <X11/Xlib.h>

int count = 0;
struct tysettings;
struct tysettings* lsettings;
int ltime;
GtkWidget *window;

static void print_hof( GtkWidget *w,
                         gpointer   data )
{
  g_message ("Hello, World!\n");
}
//
//static GtkItemFactoryEntry menu_items[] = {
//  { "/_Settings",         NULL,         NULL, 0, "	<Branch>" },
//  { "/_Hall_Of_Fame",     "<control>H", print_hof, 0, NULL },
//};

//void get_main_menu( GtkWidget  *window,
//                    GtkWidget **menubar )
//{
//  GtkItemFactory *item_factory;
//  GtkAccelGroup *accel_group;
//  gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);
//
//  accel_group = gtk_accel_group_new ();
//
//  /* This function initializes the item factory.
//     Param 1: The type of menu - can be GTK_TYPE_MENU_BAR, GTK_TYPE_MENU,
//              or GTK_TYPE_OPTION_MENU.
//     Param 2: The path of the menu.
//     Param 3: A pointer to a gtk_accel_group.  The item factory sets up
//              the accelerator table while generating menus.
//  */
//
//  item_factory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>",
//                                       accel_group);
//
//  /* This function generates the menu items. Pass the item factory,
//     the number of items in the array, the array itself, and any
//     callback data for the the menu items. */
//  gtk_item_factory_create_items (item_factory, nmenu_items, menu_items, NULL);
//
//  /* Attach the new accelerator group to the window. */
//  gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
//
////  if (menubar)
//    /* Finally, return the actual menu bar created by the item factory. */
////    *menubar = gtk_item_factory_get_widget (item_factory, "<main>");
//}
//

int greetings() {
	char lmess[50];
	double lresult = 0.00;
	GtkWindowPosition position;
	if ( ltime != 0 ) {
		lresult =(double) count / ltime;
	};
	if ( lresult < 1.9 ) {
		sprintf( lmess, "You're slow as molasses, come on!" );
	} else if ( lresult < 2.5 ) {
		sprintf( lmess, "Uhm... It seeems you need a coffe..." );
	} else if ( lresult < 4 ) {
		sprintf( lmess, "Quite good... Maybe you can play some old '80 games :)");
	} else if ( lresult < 5 ) {
		sprintf( lmess, "Very good! Fast as lightning!");
	} else sprintf( lmess, "Outstanding performance! Bravo!");

	GtkWidget *dia = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	GtkWidget *lblmsg = gtk_label_new(lmess);
	gtk_container_add (GTK_CONTAINER (dia), lblmsg);
	gtk_window_set_title(GTK_WINDOW(dia), "Clickety");
	gtk_window_set_default_size(GTK_WINDOW(dia),400,300);
	gtk_window_set_position(GTK_WINDOW(dia),position);

	gtk_widget_show_all(dia);
	gtk_widget_grab_focus(dia);
	return 0;
}

void timeout(int ignored){
   gtk_widget_hide(window);
   greetings();
}

void end_program (GtkWidget *wid, gpointer ptr){
	gtk_main_quit ();
}
void count_button (GtkWidget *wid, gpointer ptr){
	char buffer[30];
	count++;
	sprintf (buffer, "Button pressed %d times", count);
	gtk_label_set_text (GTK_LABEL (ptr), buffer);
}

void *mytimer(void *vargp) {
	signal(SIGALRM, timeout);
	alarm(ltime);
	return NULL;
}


struct tysettings {
	int time_disp;
};

int init() {
	lsettings = (struct tysettings*)
			malloc(sizeof(struct tysettings));
	lsettings->time_disp = 10;  //>> eject
	ltime = lsettings->time_disp;
	return 0;
}

GtkBuilderConnectFunc gbuild_connect_fun( GtkWidget* lbl ) {
//	g_signal_connect (window , "clicked", G_CALLBACK (count_button), lbl); //pare non servire
	g_signal_connect (window , "clicked", G_CALLBACK (print_hof), NULL);   //serve? come cambiarlo?
	return 0;
}

int function1(int argc, char **argv, int i) {
	GtkBuilder *builder;
	GtkBuilder *builder2;
	GtkWidget *lbl;
	GtkWidget *btn;
	GtkWidget *opzioni;

	gtk_init(&argc, &argv);

//    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//    GtkWidget *btn = gtk_button_new_with_label ("Click me as fast as you can");
//    GtkWidget *lbl = gtk_label_new ("Button clicked 0 times");
//    g_signal_connect (btn, "clicked", G_CALLBACK (count_button), lbl);
//    g_signal_connect (window, "delete_event", G_CALLBACK (end_program), NULL);
//    GtkWidget *box = gtk_box_new (GTK_ORIENTATION_HORIZONTAL , 0);
//    gtk_box_pack_start (GTK_BOX (box), lbl, TRUE, TRUE, 0);
//    gtk_box_pack_start (GTK_BOX (box), btn, TRUE, TRUE, 0);
//    gtk_container_add (GTK_CONTAINER (window), box);
//    gtk_window_set_title(GTK_WINDOW(window), "Clickety");
// ****    da qui nuova gestione con Glade
    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "project970_intface.glade", NULL);
    gtk_builder_add_from_resource(builder, "application_menu.xml", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    lbl = GTK_WIDGET(gtk_builder_get_object(builder, "lbl"));
    btn = GTK_WIDGET(gtk_builder_get_object(builder, "btn"));
    opzioni = GTK_WIDGET(gtk_builder_get_object(builder, "opzioni"));
    g_signal_connect (btn, "clicked", G_CALLBACK (count_button), lbl); //preso dalla vecchia gestione senza glade
    gtk_builder_connect_signals(builder, NULL);
    gtk_builder_add_callback_symbol(builder, "count_button", G_CALLBACK (btn));
    gtk_builder_connect_signals_full(builder, gbuild_connect_fun(lbl), lbl);
// ****   fine nuova gestione con Glade
    init();
    pthread_t thread_id;

    pthread_create(&thread_id, NULL, mytimer, NULL);
    pthread_join(thread_id, NULL);
    gtk_widget_show_all(window);
    gtk_main ();
    return  0;
}


int main(int argc, char **argv) {
	int i;
	i = function1( argc, argv, 1 );
	return 0;
}
