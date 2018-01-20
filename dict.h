#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"btree.h"
#include"soundex.h"
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#define M 15
#define max 80
#define maxn 1000000
int ktr=0;
int t=0;
typedef struct {
  BTA* dict,*soundex;
  GtkWidget *entry,*listview,*listview2;
  GtkTextBuffer *buffer;
}Dtype;
/*char history[M][max];
 //history[M-1]=NULL;
int Kiemtra(char history[M][max],char *tu)
{
  int i=0;
  for(i;i<M;i++){
    if(!strcmp(tu,history[i])) return -1;
  }
  if(history[M-1]==NULL) return 1;
  return 0; 
}*/

void notice(char *text)
{
  GtkWidget *window=gtk_window_new(GTK_WINDOW_POPUP);
  GtkWidget *dialog;
  dialog = gtk_message_dialog_new ((GtkWindow*)window, GTK_DIALOG_MODAL,
				   GTK_MESSAGE_INFO, GTK_BUTTONS_OK,text);
  gtk_dialog_run(GTK_DIALOG(dialog));
  gtk_widget_destroy(dialog);
}

void MakeList (GtkWidget* listview)
{
  //declear var
  GtkListStore *store;
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  //start of var
  store = gtk_list_store_new(1,G_TYPE_STRING);
  renderer = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("Goi Y",renderer,"text",0,NULL);
  gtk_tree_view_append_column(GTK_TREE_VIEW(listview),column);
  gtk_tree_view_set_model(GTK_TREE_VIEW(listview),GTK_TREE_MODEL(store));
  g_object_unref(store);
}

gboolean list_store_remove_nth_row(GtkListStore *store, gint n)
   {
     GtkTreeIter  iter;

     g_return_val_if_fail (GTK_IS_LIST_STORE(store), FALSE);

     /* NULL means the parent is the virtual root node, so the
      *  n-th top-level element is returned in iter, which is
      *  the n-th row in a list store (as a list store only has
      *  top-level elements, and no children) */
     if (gtk_tree_model_iter_nth_child(GTK_TREE_MODEL(store), &iter,NULL, n))
     {
       gtk_list_store_remove(store, &iter);
       return TRUE;
     }

     return FALSE;
   }

void Addtolist(GtkWidget *listview,char *word)
{
  GtkListStore *store;
  GtkTreeIter iter;
  store=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(listview)));
  gtk_list_store_append(store, &iter);
  gtk_list_store_set(store, &iter,0, word, -1);
}

void Addtohistory(GtkWidget *listview,char *word)
{
  int i=0;
  GtkListStore *store;
  GtkTreeIter iter;
  store=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(listview)));
  //if(Kiemtra(history,word)==1)
  if(ktr <15)
  {
     //for(i;i<M;i++){
       // if(history[i]==NULL){ strcpy(history[i],word); break; }
     //}
     gtk_list_store_prepend(store, &iter);
     gtk_list_store_set(store, &iter,0, word, -1);
     ktr++;
	//printf("ktr: %d\n",ktr);
  }else{
     gtk_list_store_prepend(store, &iter);
     gtk_list_store_set(store, &iter,0, word, -1);
     //gtk_list_store_move_before(store, &iter,NULL);
     //gtk_list_store_remove(store, &iter);	
     list_store_remove_nth_row(store,15);
     //printf("ktr: %s\n",word);
  }
}

void Themtu(GtkWidget* widget,gpointer data)
{
  //change type
  Dtype* DICT = (Dtype*)data;
  //create interface
  //declear var
  GtkWidget* dialog,*label;
  GtkWidget* word_entry,*mean_tv;
  GtkWidget* word,*mean;
  GtkWidget* add,*cancel;
  GtkWidget *scrolled_window;
  GtkWidget *table;
  GtkTextBuffer* buffer1;
  GtkTextIter start,end;
  //start create interface
  dialog = gtk_dialog_new_with_buttons ("Thêm từ",NULL,GTK_DIALOG_MODAL,GTK_STOCK_OK,1,GTK_STOCK_CANCEL,2,NULL);
  //g_signal_connect (dialog, "delete-event",G_CALLBACK(gtk_widget_destroy),NULL);
  label = gtk_label_new("Hãy nhập từ và nghĩa");
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),label,0,0,0);
  gtk_window_set_default_size(GTK_WINDOW(dialog),500,200);
  word = gtk_label_new("Từ");
  mean = gtk_label_new("Nghĩa");
  word_entry = gtk_entry_new();
  mean_tv = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(mean_tv),TRUE);
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(mean_tv),10);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(mean_tv),10);
  buffer1 = gtk_text_view_get_buffer(GTK_TEXT_VIEW(mean_tv));
  scrolled_window = gtk_scrolled_window_new(NULL,NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
  gtk_container_add(GTK_CONTAINER(scrolled_window),mean_tv);
  //create table to locate
  table = gtk_table_new(5,5,0);
  gtk_table_attach_defaults(GTK_TABLE(table),word,0,1,0,1);
  gtk_table_attach_defaults(GTK_TABLE(table),mean,0,1,1,4);
  gtk_table_attach_defaults(GTK_TABLE(table),word_entry,1,4,0,1);
  gtk_table_attach_defaults(GTK_TABLE(table),scrolled_window,1,4,1,4);
  gtk_container_set_border_width(GTK_CONTAINER(mean_tv),1);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_window),0);
  gtk_container_set_border_width (GTK_CONTAINER (table),2);
  gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox),table,TRUE,TRUE,0);
  gtk_widget_show_all(dialog);
  //start program
  gint response = gtk_dialog_run(GTK_DIALOG(dialog));

  char tu[max],nghia[maxn],buffer[maxn];
  char *s1=NULL,*s2=NULL,*snul=NULL;
  char soundex_key[max]; 
  int len,i,realsize,len_nghia;
  while(response == 1)
    {
      gtk_text_buffer_get_bounds(buffer1,&start,&end);
      s1 = gtk_entry_get_text(GTK_ENTRY(word_entry));
      s2  = gtk_text_buffer_get_text(buffer1,&start,&end,0);
      //strcpy(tu,s1);strcpy(nghia,s2);

      //nghia[strlen'(nghia)]='\0';
      if (strcmp(s1,"")!=0)
    {
      len=strlen(s1);
      for (i=0;i<len;i++)
        if (s1[i]>'9'&&s1[i]<'a') s1[i]=tolower(s1[i]);
      SoundEx(soundex_key,s1,0,0);
      //printf("%s\n%s\n",soundex_key,s1);
      if (!btsel(DICT->dict,s1,buffer,maxn,&realsize))
        {

          notice("Từ đã tồn tại!");

        }
      else
        {
          //len=strlen(s1);
          len_nghia=strlen(s2);
	  btins(DICT->dict,s1,s2,(len_nghia+1)*sizeof(char));
	  notice("Đã thêm từ thành công!");
	  gtk_entry_set_text(GTK_ENTRY(word_entry),"");
	  gtk_text_buffer_set_text(buffer1,"",-1);

      /*strcpy(tu,s1);
      strcat(tu,"\n");
	  if (btsel(DICT->soundex,soundex_key,buffer,maxn,&realsize))
	    {
	      btins(DICT->soundex,soundex_key,tu,(len+2)*sizeof(char));
	    }
	  else
	    {
	      btupd(DICT->soundex,soundex_key,tu,(len+2)*sizeof(char));
	    }
      */
	}
    }
  else
    {
      notice("Chưa nhập từ!");

    }
  response = gtk_dialog_run(GTK_DIALOG(dialog));
    }
   gtk_widget_destroy(GTK_WIDGET(dialog));
}

void Goiy(GtkWidget *widget,gpointer data)
{
  Dtype* DICT = (Dtype*)data;

  char *tu=(char*)malloc(max);
  char *nghia=(char*)malloc(maxn);
  char soundex_key[max],tu_soundex[max],s_tu[max],soundex_key1[max],tu1[max];
  int i,realsize;
  GtkListStore *store;
  char *word=gtk_entry_get_text((GtkEntry*)DICT->entry);
  for (i=0;i<strlen(word);i++)
    if (word[i]>'9'&&word[i]<'a') word[i]=tolower(word[i]);

  gtk_entry_set_text((GtkEntry*)DICT->entry,word);
  store=GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(DICT->listview)));

  //printf("%s\n",word);
  strcpy(tu,word);
  s_tu[0]=tu[0]; s_tu[1]='\0';

  gtk_list_store_clear(store);
	SoundEx(soundex_key,tu,0,0);
  //if (btsel(DICT->dict,tu,nghia,maxn,&realsize)!=0)
    //{
      
      //if(!btsel(DICT->soundex,soundex_key,tu_soundex,maxn,&realsize))
	
	if (btsel(DICT->dict,s_tu,nghia,maxn,&realsize)==0)

	  {

	    while(btseln(DICT->dict,tu1,nghia,maxn,&realsize)==0)
	      {
		SoundEx(soundex_key1,tu1,0,0);
		if (strcmp(soundex_key,soundex_key1)==0)
		  Addtolist(DICT->listview,tu1);
		if(tu1[0]!=s_tu[0]) break;

	      }
	  }

    //}
  /*else
    {

      //Addtolist(DICT->listview,tu);
      while (btseln(DICT->dict,tu,nghia,maxn,&realsize)==0)
	{
	  i=strncmp(tu,word,strlen(word));
	  if(i!=0) break;
	  Addtolist(DICT->listview,tu);
	  //i++;
	}
    }*/
  free(tu);
  free(nghia);

}

void Thaydoi(GtkWidget *widget,gpointer data)
{
  Dtype *DICT=(Dtype*)data;
  GtkTreeIter iter;
  GtkTreeModel *model;
  int j,realsize;
  char *tu=(char*)malloc(100);
  char nghia[maxn];
  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter))
    gtk_tree_model_get(model, &iter,0, &tu, -1);
  btsel(DICT->dict,tu,nghia,maxn,&realsize);
  //printf("ktr: %s\n",tu);
	//Addtohistory(DICT->listview2,tu);
  //gtk_entry_set_text((GtkEntry*)(DICT->entry),"");
  //gtk_entry_set_text((GtkEntry*)(DICT->entry),tu);
  gtk_text_buffer_set_text(DICT->buffer,nghia,-1);
  g_free(tu);
}

gboolean Hoanthanh(GtkWidget *widget,GdkEventKey *event,gpointer data)
{
  Dtype *DICT=(Dtype*)data;
  char *tu;
  char word1[max],nghia[maxn];
  int realsize,i;
  if((event->keyval) != GDK_Tab) 
  {	
	//gtk_entry_set_text((GtkEntry*)DICT->entry,"fasle");     
	return FALSE;
  }
  btpos(DICT->dict,1);
  tu=gtk_entry_get_text((GtkEntry*)DICT->entry);
  //gtk_entry_set_text((GtkEntry*)DICT->entry,tu);

  int len=strlen(tu);
  for (i=0;i<len;i++)    
	if (tu[i]>'9'&&tu[i]<'a') tu[i]=tolower(tu[i]);
  strcpy(word1,tu);

  if (btsel(DICT->dict,word1,nghia,maxn,&realsize)!=0)
    {
      btins(DICT->dict,word1,nghia,maxn);
      btsel(DICT->dict,word1,nghia,maxn,&realsize);
      btseln(DICT->dict,word1,nghia,maxn,&realsize);
      //printf("%s\n",word1);
      if (strncmp(word1,tu,len)==0)
	{
		gtk_entry_set_text((GtkEntry*)(DICT->entry),word1);
	}
      btsel(DICT->dict,word1,nghia,maxn,&realsize);
      btselp(DICT->dict,word1,nghia,maxn,&realsize);
      btdel(DICT->dict,word1);
      
    }
  else {
	btsel(DICT->dict,word1,nghia,maxn,&realsize);      
	btseln(DICT->dict,word1,nghia,maxn,&realsize);
	if (strncmp(word1,tu,len)==0)
	{
		gtk_entry_set_text((GtkEntry*)(DICT->entry),word1);
	}	
	else gtk_entry_set_text((GtkEntry*)(DICT->entry),tu);
	}
  return TRUE;
}

void Tratu(GtkWidget *widget, gpointer data)
{
  Dtype* DICT = (Dtype*)data;
  char tu[max],nghia[maxn];
  char *s1,*s2;
  char soundex_key[max];
  int i,realsize;
  char *s=gtk_entry_get_text((GtkEntry*)DICT->entry);
  if (strcmp(s,"")!=0)
    {
      for (i=0;i<strlen(s);i++)
	if (s[i]>'9'&&s[i]<'a') s[i]=tolower(s[i]);
      //printf("%s",s);
      strcpy(tu,s);

      if (btsel(DICT->dict,tu,nghia,maxn,&realsize)!=0)
	{


	  gtk_text_buffer_set_text(DICT->buffer,"Không tìm thấy từ!",-1);
	}
      else{
	gtk_text_buffer_set_text(DICT->buffer,nghia,-1);
	Addtohistory(DICT->listview2,tu);	
	}
    }
}

void Suatu(GtkWidget *widget,gpointer data)
{
  Dtype *DICT=(Dtype*)data;
  GtkWidget *dialog;
  GtkWidget *word,*mean;
  GtkWidget *word_entry,*mean_text;
  dialog=gtk_dialog_new_with_buttons("Sửa từ",NULL,GTK_DIALOG_MODAL,GTK_STOCK_OK,1,GTK_STOCK_CANCEL,2,NULL);
  g_signal_connect (dialog, "delete-event",G_CALLBACK(gtk_widget_destroy),NULL);
  gtk_window_set_title(GTK_WINDOW(dialog),"Sửa từ");
  gtk_window_set_default_size(GTK_WINDOW(dialog), 500, 250);
  word=gtk_label_new("Từ");
  mean=gtk_label_new("Nghĩa");
  word_entry=gtk_entry_new();
  mean_text=gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(mean_text),TRUE);
  gtk_text_view_set_left_margin(GTK_TEXT_VIEW(mean_text),10);
  gtk_text_view_set_right_margin(GTK_TEXT_VIEW(mean_text),10);
  GtkTextBuffer *buffer;
  buffer=gtk_text_view_get_buffer(GTK_TEXT_VIEW(mean_text));
  GtkWidget *scrolled_win=gtk_scrolled_window_new(NULL,NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_win),GTK_POLICY_AUTOMATIC,GTK_POLICY_ALWAYS);
  gtk_container_add(GTK_CONTAINER(scrolled_win),mean_text);
  GtkWidget *table;
  table=gtk_table_new(1,1,TRUE);
  gtk_table_attach_defaults(GTK_TABLE(table),word,0,1,0,1);
  gtk_table_attach_defaults(GTK_TABLE(table),mean,0,1,1,4);
  gtk_table_attach_defaults(GTK_TABLE(table),word_entry,1,4,0,1);
  gtk_table_attach_defaults(GTK_TABLE(table),scrolled_win,1,4,1,4);
  gtk_table_set_row_spacings (GTK_TABLE (table),0);
  gtk_table_set_col_spacings (GTK_TABLE (table),0);
  gtk_container_set_border_width(GTK_CONTAINER(mean_text),1);
  gtk_container_set_border_width(GTK_CONTAINER(scrolled_win),1);
  gtk_container_set_border_width (GTK_CONTAINER (table),2);
  gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),table);
  gtk_widget_show_all(dialog);
  GtkTextIter start,end;

  char *tu,*nghia,s2[maxn];
  int realsize;
  int result=gtk_dialog_run(GTK_DIALOG(dialog));
  while(result==1)
    {
      gtk_text_buffer_get_bounds(buffer,&start,&end);
      tu=gtk_entry_get_text((GtkEntry*)word_entry);
      nghia=gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
      if (strcmp(tu,"")!=0)
	{

	  if (btsel(DICT->dict,tu,s2,maxn,&realsize)!=0)
	    notice("Không tìm thấy từ cần sửa !");
	  else
	    {

	      btupd(DICT->dict,tu,nghia,(strlen(nghia)+1)*sizeof(char));
	      notice("Đã sửa !");
	      gtk_entry_set_text((GtkEntry*)word_entry,"");
	      gtk_text_buffer_set_text(buffer,"",-1);
	    }
	}
      else
	notice("Chưa nhập từ !");
      result=gtk_dialog_run(GTK_DIALOG(dialog));
    }
  gtk_widget_destroy(dialog);
}

void Xoatu(GtkWidget* widget, gpointer data )
{
  Dtype* DICT = (Dtype*)data;
  char *tu,nghia[maxn],buffer[maxn],tu2[max];
  char soundex_key[max];
  char tu_soundex[max],thay_soundex[max];
  int i,realsize;
  GtkWidget *dialog;
  GtkWidget *word_entry,*label;
  //GtkTextBuffer *buffer;
  dialog=gtk_dialog_new_with_buttons("Xóa từ",NULL,GTK_DIALOG_MODAL,GTK_STOCK_OK,1,GTK_STOCK_CANCEL,2,NULL);
  g_signal_connect (dialog, "delete-event",G_CALLBACK(gtk_widget_destroy),NULL);
  //gtk_window_set_title(GTK_WINDOW(dialog),"Xóa từ");
  label=gtk_label_new("Nhập từ cần xóa");
  word_entry=gtk_entry_new_with_max_length(200);
  gtk_window_set_default_size(GTK_WINDOW(dialog),350, 150);
  GtkWidget *table;
  table=gtk_table_new(1,1,TRUE);
  gtk_table_attach_defaults(GTK_TABLE(table),label,0,1,0,2);
  gtk_table_attach_defaults(GTK_TABLE(table),word_entry,0,1,3,5);
  gtk_table_set_row_spacings (GTK_TABLE (table),1);
  gtk_table_set_col_spacings (GTK_TABLE (table),1);
  gtk_container_set_border_width (GTK_CONTAINER (table),10);
  gtk_box_pack_start_defaults(GTK_BOX(GTK_DIALOG(dialog)->vbox),table);
  gtk_widget_show_all(dialog);
  int result=gtk_dialog_run(GTK_DIALOG(dialog));
  int len,len_sd;
  while(result==1)
    {
      tu=gtk_entry_get_text((GtkEntry*)word_entry);
      if (strcmp(tu,"")!=0)
	{
      len=strlen(tu);
	  for (i=0;i<len;i++)
	    if (tu[i]>'9'&&tu[i]<'a') tu[i]=tolower(tu[i]);
	  SoundEx(soundex_key,tu,0,0);


	  if (!btsel(DICT->dict,tu,buffer,maxn,&realsize))
	    {
           //printf("%s\n",buffer);
          /*strcpy(tu2,tu);
	      strcat(tu2,"\n");
	      btpos(DICT->dict,1);
	      while (btseln(DICT->dict,tu_soundex,nghia,maxn,&realsize)==0)
		{
		  SoundEx(thay_soundex,tu_soundex,0,0);
		  strcat(tu_soundex,"\n");
		  if ((strcmp(thay_soundex,soundex_key)==0)&&(strcmp(tu_soundex,tu2)!=0))
		    {
              len_sd=strlen(tu_soundex);
		      //printf("%s\n%s\n",thay_soundex,tu_soundex);
		      btupd(DICT->soundex,thay_soundex,tu_soundex,(len_sd+1)*sizeof(char));
		      break;
		    }
		}
	      btsel(DICT->soundex,soundex_key,buffer,maxn,&realsize);
	      //printf("%s\n",buffer);
	      if (strcmp(buffer,tu2)==0)
		btdel(DICT->soundex,soundex_key);
        tu2[len-1]='\0';*/
	      btdel(DICT->dict,tu);
	      notice("Đã xóa từ!\n");
          gtk_entry_set_text((GtkEntry*)word_entry,"");
	    }
	  else
	    {
	      notice("Không tìm thấy từ cần xóa!\n");
	    }

	}
      else
	{
	  notice("Chưa nhập từ!\n");
	}
      result=gtk_dialog_run(GTK_DIALOG(dialog));
    }
  gtk_widget_destroy(GTK_WIDGET(dialog));
}


