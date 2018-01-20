/*
Bien dich chuong trinh

gcc dict.c -Llib -Iinc -lbt -lm -o dict `pkg-config --libs gtk+-2.0` `pkg-config --cflags gtk+-2.0`

*/

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "dict2.h"
	
#define entry DICT->entry
#define buffer DICT->buffer
#define listview DICT->listview
#define listview2 DICT->listview2


/*void Lichsu(GtkWidget* widget,gpointer data){
	Dtype* DICT = (Dtype*)data;
	GtkWidget* dialog;
	GtkWidget* listview_window;
	GtkWidget* hbox_his;
	//GtkWidget* add, *cancel;
	
	dialog = gtk_dialog_new_with_buttons ("Lich su tra tu",NULL,GTK_DIALOG_MODAL,GTK_STOCK_CANCEL,1,NULL);
	gtk_window_set_default_size(GTK_WINDOW(dialog),200,500);
	//create a view list ( danh sach tu goi y)
  	listview = gtk_tree_view_new();
  	gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(listview),1);
  	//gtk_box_pack_start(GTK_BOX(vbox_his),listview,FALSE,FALSE,5);
//tao scroll cho list view 
  	listview_window=gtk_scrolled_window_new(NULL,NULL);
  	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(listview_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(listview_window), GTK_SHADOW_ETCHED_IN);
  	gtk_container_add(GTK_CONTAINER(listview_window),listview);
  	gtk_box_pack_start(GTK_BOX(hbox_his),listview_window,0,0,0);
  	MakeList(listview);

	gtk_widget_show_all(dialog);
	gint response = gtk_dialog_run(GTK_DIALOG(dialog));	
	if (response == 1)	
		gtk_widget_destroy(dialog);

}*/

int main (int argc, char* argv[])
{
  gtk_init(&argc,&argv);    //initialize the gtk
  //start declear var
  GtkWidget* window,*listview_window;
  GtkWidget* search,*add,*change,*delete;
  GtkWidget* hbox;
  GtkWidget* vbox_entry,*hbox_button,*vbox_button,*vbox1,*vbox2,*vbox3,*vbox4,*vbox5,*vbox6;

  GtkTreeSelection *selection,*selection2;
  GtkWidget* ic1,*ic2,*ic3,*ic4;
  Dtype* DICT = g_slice_new(Dtype);
  btinit();
  DICT->dict = btopn ("dictionary.dat",0,0);
  //DICT->soundex = btopn ("soundex.dat",0,0);
  buffer = gtk_text_buffer_new(NULL);
  //create window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_default_size(GTK_WINDOW(window),700,400);
  gtk_container_set_border_width(GTK_CONTAINER(window),5);
  gtk_window_set_title(GTK_WINDOW(window),"Tu dien anh viet");
  gtk_window_set_position(GTK_WINDOW(window),GTK_WIN_POS_CENTER);
  g_signal_connect (window, "delete-event",G_CALLBACK(gtk_main_quit),NULL); //if X button clicked,closed program
  
  //from here start the main program

  //create box
  hbox = gtk_hbox_new(0,20);
  vbox1 = gtk_vbox_new(0,2);
  vbox2 = gtk_vbox_new(0,3);
  vbox3 = gtk_vbox_new(0,3);
  vbox4 = gtk_vbox_new(0,3);
  vbox5 = gtk_vbox_new(0,3);
  vbox6 = gtk_vbox_new(0,2);
  hbox_button = gtk_hbox_new(0,3);
  vbox_button = gtk_vbox_new(0,3);
	

  //create text entry ( de nhap tu can tra)	
  entry = gtk_entry_new_with_max_length(100);

  gtk_box_pack_start(GTK_BOX(vbox1),entry,0,0,2);
  g_signal_connect(entry,"activate",G_CALLBACK(Tratu),(gpointer)DICT);
  
  g_signal_connect(entry,"changed",G_CALLBACK(Goiy),(gpointer)DICT);
  g_signal_connect(entry,"key-press-event",G_CALLBACK(Hoanthanh),(gpointer)DICT);
  //create a view list ( danh sach tu goi y)
  listview = gtk_tree_view_new();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(listview),1);
  //gtk_box_pack_start(GTK_BOX(vbox1),listview,FALSE,FALSE,5);
  selection=gtk_tree_view_get_selection(GTK_TREE_VIEW(listview));
  g_signal_connect(selection, "changed", G_CALLBACK(Thaydoi),(gpointer)DICT);
  
  //tao scroll cho list view 
  listview_window=gtk_scrolled_window_new(NULL,NULL);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW(listview_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW(listview_window), GTK_SHADOW_ETCHED_IN);
  gtk_container_add(GTK_CONTAINER(listview_window),listview);
  gtk_box_pack_start(GTK_BOX(vbox1),listview_window,1,1,5);
  MakeList(listview);
	
 
 //create search button
  search = gtk_button_new();
  gtk_widget_show(search);
  g_signal_connect(search,"clicked",G_CALLBACK(Tratu),(gpointer)DICT);
  gtk_box_pack_start(GTK_BOX(vbox2),search,0,0,3);
  ic1 = gtk_image_new_from_file("icons/search-icon.png");
  gtk_image_set_pixel_size(GTK_IMAGE(ic1),2);
  gtk_container_add(GTK_CONTAINER(search),ic1);
  gtk_widget_show(ic1);

  //create add button
  add = gtk_button_new();
  gtk_widget_show(add);
  g_signal_connect(add,"clicked",G_CALLBACK(Themtu),(gpointer)DICT);
  gtk_box_pack_start(GTK_BOX(vbox3),add,0,0,3);
  ic2 = gtk_image_new_from_file("icons/edit-icon.png");
  gtk_container_add(GTK_CONTAINER(add),ic2);
  gtk_widget_show(ic2);

  //create change button
  change = gtk_button_new();
  gtk_widget_show(change);
  g_signal_connect(change,"clicked",G_CALLBACK(Suatu),(gpointer)DICT);
  gtk_box_pack_start(GTK_BOX(vbox4),change,0,0,3);
  ic3 = gtk_image_new_from_file("icons/change-icon.png");
  gtk_container_add(GTK_CONTAINER(change),ic3);
  gtk_widget_show(ic3);

  //create delete button
  delete = gtk_button_new();
  gtk_widget_show(delete);
  g_signal_connect(delete,"clicked",G_CALLBACK(Xoatu),(gpointer)DICT);
  gtk_box_pack_start(GTK_BOX(vbox5),delete,0,0,3);
  ic4 = gtk_image_new_from_file("icons/trash-icon.png");
  gtk_container_add(GTK_CONTAINER(delete) ,ic4);
  gtk_widget_show(ic4);

  //pack button box
  gtk_box_pack_start(GTK_BOX(hbox_button),vbox2,0,0,3);
  gtk_box_pack_start(GTK_BOX(hbox_button),vbox3,0,0,3);
  gtk_box_pack_start(GTK_BOX(hbox_button),vbox4,0,0,3);
  gtk_box_pack_start(GTK_BOX(hbox_button),vbox5,0,0,3);

  gtk_box_pack_start(GTK_BOX(vbox_button),hbox_button,0,0,3);

  //Creat text view (de hien nghia)
  GtkWidget *view;

  view=gtk_text_view_new();
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (view), GTK_WRAP_WORD);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(view),FALSE);
  gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view),FALSE);
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(view),10);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(view),10);
  buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
  GtkWidget *scrolled_win=gtk_scrolled_window_new(NULL,NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
  gtk_container_add(GTK_CONTAINER(scrolled_win),view);
  gtk_box_pack_start(GTK_BOX(vbox_button),scrolled_win,TRUE,TRUE,5);

 //Create box history
 GtkWidget *label2,*list;
 label2 = gtk_label_new("Lich Su Tim Kiem");
 //create a view list ( danh sach tu goi y)
  listview2 = gtk_tree_view_new();
  gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(listview2),0);
 //gtk_box_pack_start(GTK_BOX(vbox1),listview,FALSE,FALSE,5);
  selection2=gtk_tree_view_get_selection(GTK_TREE_VIEW(listview2));
  g_signal_connect(selection2, "changed", G_CALLBACK(Thaydoi),(gpointer)DICT);
 gtk_box_pack_start(GTK_BOX(vbox6),label2,0,1,0);
 gtk_box_pack_start(GTK_BOX(vbox6),listview2,TRUE,TRUE,5);
  MakeList(listview2);
 //pack all box

  gtk_box_pack_start(GTK_BOX(hbox),vbox1,0,1,5);

  gtk_box_pack_start(GTK_BOX(hbox),vbox_button,0,1,5);

  gtk_box_pack_start(GTK_BOX(hbox),vbox6,1,TRUE,5);
  //end of program
  gtk_container_add(GTK_CONTAINER(window),hbox);
  gtk_widget_show_all(window); //show all widgets
  gtk_main();//start the main loop
  btcls(DICT->dict);
  btcls(DICT->soundex);
  return 0;
}
