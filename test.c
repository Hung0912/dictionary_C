#include <gtk/gtk.h>


void destroy (void)
{
  gtk_main_quit ();
}

/* Make a new hbox filled with button-labels. Do not show the box, but do
   show everything inside. */
GtkWidget *make_box (gint homogeneous, gint spacing,
		     gint expand, gint fill, gint padding) {
  GtkWidget *box;
  GtkWidget *button;
  char padstr[80];

  box = gtk_hbox_new (homogeneous, spacing);

  button = gtk_button_new_with_label ("gtk_box_pack");
  gtk_box_pack_start (GTK_BOX (box), button, expand, fill, padding);
  gtk_widget_show (button);

  button = gtk_button_new_with_label ("(box,");
  gtk_box_pack_start (GTK_BOX (box), button, expand, fill, padding);
  gtk_widget_show (button);

  button = gtk_button_new_with_label ("button,");
  gtk_box_pack_start (GTK_BOX (box), button, expand, fill, padding);
  gtk_widget_show (button);

  button = gtk_button_new_with_label (expand ? "TRUE," : "FALSE,");
  gtk_box_pack_start (GTK_BOX (box), button, expand, fill, padding);
  gtk_widget_show (button);

  button = gtk_button_new_with_label (fill ? "TRUE," : "FALSE,");
  gtk_box_pack_start (GTK_BOX (box), button, expand, fill, padding);
  gtk_widget_show (button);

  sprintf (padstr, "%d);", padding);

  button = gtk_button_new_with_label (padstr);
  gtk_box_pack_start (GTK_BOX (box), button, expand, fill, padding);
  gtk_widget_show (button);

  return box;
}

int
main (int argc, char *argv[])
{
  GtkWidget *window;
  GtkWidget *button;
  GtkWidget *box1;
  GtkWidget *box2;
  GtkWidget *separator;
  GtkWidget *label;
  GtkWidget *quitbox;
  int which;

  gtk_init (&argc, &argv);

  if (argc != 2) {
    fprintf (stderr, "usage: packbox num, where num is 1, 2, or 3.\n");
    gtk_exit (1);
  }

  which = atoi (argv[1]);

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect (GTK_OBJECT (window), "destroy",
		      GTK_SIGNAL_FUNC (destroy), NULL);
  gtk_container_border_width (GTK_CONTAINER (window), 10);

  box1 = gtk_vbox_new (FALSE, 0);

  switch (which) {
  case 1:
    label = gtk_label_new ("gtk_hbox_new (FALSE, 0);");
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
    gtk_box_pack_start (GTK_BOX (box1), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    box2 = make_box (FALSE, 0, FALSE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, FALSE, 0);
    gtk_widget_show (box2);
    
    box2 = make_box (FALSE, 0, TRUE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, FALSE, 0);
    gtk_widget_show (box2);
    
    box2 = make_box (FALSE, 0, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, FALSE, 0);
    gtk_widget_show (box2);
    
    separator = gtk_hseparator_new ();
    gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 5);
    gtk_widget_show (separator);
    
    label = gtk_label_new ("gtk_hbox_new (TRUE, 0);");
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
    gtk_box_pack_start (GTK_BOX (box1), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    box2 = make_box (TRUE, 0, TRUE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, FALSE, 0);
    gtk_widget_show (box2);
    
    box2 = make_box (TRUE, 0, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, FALSE, 0);
    gtk_widget_show (box2);
    
    separator = gtk_hseparator_new ();
    gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 5);
    gtk_widget_show (separator);
  
    break;
  case 2:

    label = gtk_label_new ("gtk_hbox_new (FALSE, 10);");
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
    gtk_box_pack_start (GTK_BOX (box1), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    box2 = make_box (FALSE, 10, TRUE, FALSE, 0);
    gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, FALSE, 0);
    gtk_widget_show (box2);
    
    box2 = make_box (FALSE, 10, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, FALSE, 0);
    gtk_widget_show (box2);
    
    separator = gtk_hseparator_new ();
    gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 5);
    gtk_widget_show (separator);
    
    label = gtk_label_new ("gtk_hbox_new (FALSE, 0);");
    gtk_misc_set_alignment (GTK_MISC (label), 0, 0);
    gtk_box_pack_start (GTK_BOX (box1), label, FALSE, FALSE, 0);
    gtk_widget_show (label);
    
    box2 = make_box (FALSE, 0, TRUE, FALSE, 10);
    gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, FALSE, 0);
    gtk_widget_show (box2);
    
    box2 = make_box (FALSE, 0, TRUE, TRUE, 10);
    gtk_box_pack_start (GTK_BOX (box1), box2, FALSE, FALSE, 0);
    gtk_widget_show (box2);
    
    separator = gtk_hseparator_new ();
    gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 5);
    gtk_widget_show (separator);
    break;
  case 3:
    box2 = make_box (FALSE, 0, FALSE, FALSE, 0);
    label = gtk_label_new ("start");
    gtk_box_pack_end (GTK_BOX (box2), label, FALSE, FALSE, 0);
    gtk_widget_show (label);

    gtk_box_pack_end (GTK_BOX (box1), box2, FALSE, FALSE, 0);
    gtk_widget_show (box2);

    separator = gtk_hseparator_new ();
    gtk_widget_set_usize (separator, 400, 5);
    gtk_box_pack_start (GTK_BOX (box1), separator, FALSE, TRUE, 5);
    gtk_widget_show (separator);    
  }

  quitbox = gtk_hbox_new (FALSE, 0);

  button = gtk_button_new_with_label ("Quit");

  gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
			     GTK_SIGNAL_FUNC (gtk_widget_destroy),
			     GTK_OBJECT (window));
  gtk_box_pack_start (GTK_BOX (quitbox), button, TRUE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (box1), quitbox, FALSE, FALSE, 0);
  gtk_container_add (GTK_CONTAINER (window), box1);

  gtk_widget_show (button);
  gtk_widget_show (quitbox);

  gtk_widget_show (box1);
  gtk_widget_show (window);

  gtk_main ();

  return 0;
}
