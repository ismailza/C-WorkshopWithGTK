

void chks_field_set_owner(ChksField *field)
{
    GtkCssProvider* provider;
    provider = gtk_css_provider_new();

    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider),__STYLE__,NULL);
    GtkStyleContext *context;
    context = gtk_widget_get_style_context(field->button);
    switch (field->owner) {
        case CHKS_OWNER_NONE:
            gtk_style_context_remove_class(context, "chks-player-one");
            gtk_style_context_remove_class(context, "chks-player-two");
            gtk_style_context_remove_class(context, "chks-player-one-quenn");
            gtk_style_context_remove_class(context, "chks-player-two-quenn");
            break;
        case CHKS_OWNER_ONE:
            gtk_style_context_remove_class(context, "chks-player-one-pre");
            if(field->is_queen)gtk_style_context_add_class(context, "chks-player-one-quenn");
            else gtk_style_context_add_class(context, "chks-player-one");
            break;
        case CHKS_OWNER_TWO:
            gtk_style_context_remove_class(context, "chks-player-two-pre");
            if(field->is_queen)gtk_style_context_add_class(context, "chks-player-two-quenn");
            else gtk_style_context_add_class(context, "chks-player-two");
            break;
    }
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void apply_style_button(ChksField *field[8][8])
{
    int i,j;
    for(i=0;i<8;i++)
        for(j=0;j<8;j++)
            chks_field_set_owner(field[i][j]);
}

void piece_entered(GtkWidget *button,ChksField *field)
{
    if(!field->view_table->table->last)return;
    if(field->owner != CHKS_OWNER_NONE)return;
    if(techek_entred_piece(field,field->view_table->table->last,NULL))
    {
        GtkStyleContext* context = gtk_widget_get_style_context(button);
        GtkCssProvider* provider = gtk_css_provider_new();
        gtk_css_provider_load_from_path(provider, __STYLE__, NULL);
        switch (field->view_table->table->last->owner) {

            case CHKS_OWNER_ONE:
                gtk_style_context_add_class(GTK_STYLE_CONTEXT(context),"chks-player-one-pre");
                break;
            case CHKS_OWNER_TWO:
                gtk_style_context_add_class(GTK_STYLE_CONTEXT(context),"chks-player-two-pre");
                break;
        }
        gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
}

void piece_cliecked(GtkWidget *button, ChksField *fieled)
{
    if(fieled->view_table->user->nbr_piece_disp==0 ||
    fieled->view_table->machine->nbr_piece_disp==0)
        return;//l'un des deux joueur a gagné
    if(fieled->owner==CHKS_OWNER_TWO)return;//on peut pas jouer avec les pieces de la machine
    if(fieled->view_table->table->last)
    {
        if(fieled->owner==CHKS_OWNER_ONE)
        {
            vider_file(fieled->view_table->g->f);
            fieled->view_table->table->last=fieled;
            return;
        }
        //field NONE
        cellule *ne= creer_initialiser_cellule(fieled->row,
                                               fieled->col,-1,-1);
        enfiler(fieled->view_table->g->f,ne);
        fieled->view_table->g->field=fieled;//le dernier None clicker
    }else fieled->view_table->table->last=fieled;
}

void piece_leaved(GtkWidget *button,ChksField *field)
{
    GtkStyleContext* context;
    context = gtk_widget_get_style_context(button);
    gtk_style_context_remove_class(context, "chks-player-one-pre");
    gtk_style_context_remove_class(context, "chks-player-two-pre");
}

gboolean effectuer_moumvment(GtkWidget* self,GdkEventKey event,Glob *g)
{
    GtkStyleContext* context;
    if(event.keyval==GDK_KEY_e)
    {

        if(g->f->tete==NULL) return TRUE;
        if(g->f->tete==g->f->queue)//un seule deplacement
        {
            if(techek_entred_piece(g->field,g->field->view_table->table->last,g->f))
            {
                //deplacement normal , il faut soustraire les piece attraper et traiter le cas de is queen
                // and change style
                drop_two_betwen_fl_lt(g->field->view_table->table->last,g->field,NULL,CHKS_OWNER_TWO);
            }else return (int)0;
        }else//plusierurs deplacement
        {
            File *ftmp=creer_initaliser_file();
            copy_file(g->f,ftmp);
            techek_entred_piece(g->field,g->field->view_table->table->last,g->f);
            if(g->f->tete==g->f->queue)
            {
                //deplacement long , il faut soustraire les piece attraper et traiter le cas de is queen
                drop_two_betwen_fl_lt(g->field->view_table->table->last,g->field,ftmp,CHKS_OWNER_TWO);
            }else return (int)0;
        }
        if(g->field->row==8)
        {
            g->field->is_queen=TRUE;
            g->field->view_table->table->last->is_queen=FALSE;
        }
        g->field->view_table->table->last->owner=CHKS_OWNER_NONE;
        g->field->owner=CHKS_OWNER_ONE;
        if(g->field->view_table->table->last->is_queen)
        {
            g->field->is_queen=TRUE;
            g->field->view_table->table->last->is_queen=FALSE;
        }
        apply_style_button(g->field->view_table->table->filed);
        //changer row and col de field dans user ,
        change_row_col_piece(NULL,g->field,g->field->view_table->table->last->row,
                             g->field->view_table->table->last->col,CHKS_OWNER_ONE);
        g->field->view_table->table->last=NULL;
        arbre_descicion(g);
        if(g->field->view_table->user->nbr_piece_disp==0)
        {
            gtk_header_bar_set_title(GTK_HEADER_BAR(g->field->view_table->header_bar),"BOOT Win");
        }else if(g->field->view_table->machine->nbr_piece_disp==0) {
            gtk_header_bar_set_title(GTK_HEADER_BAR(g->field->view_table->header_bar), "User Win");
        }
        vider_file(g->f);
        g->field=NULL;
    }
    return (TRUE);
}

void add_filed_grid(Chkswindow *win,ChksField *fieled[8][8])
{
    GtkAccelGroup *accel_group;
    accel_group = gtk_accel_group_new ();
    gtk_window_add_accel_group (GTK_WINDOW (win->win), accel_group);
    GtkStyleContext* context;
    GtkCssProvider* provider;
    context = gtk_widget_get_style_context(win->table->gr);
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(GTK_CSS_PROVIDER(provider),__STYLE__,NULL);
    gtk_style_context_add_class(GTK_STYLE_CONTEXT(context),"chks-grid");
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_grid_set_column_homogeneous(GTK_GRID(win->table->gr), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(win->table->gr), TRUE);
    int i,j;
    for(i=0;i<=7;i++) {
        for (j = 0; j <= 7; j++)
        {
            //j is column and i is row
            fieled[i][j]->view_table = win;
            gtk_grid_attach(GTK_GRID(win->table->gr), fieled[i][j]->button, j, i, 1, 1);
            g_signal_connect(fieled[i][j]->button,"clicked",G_CALLBACK(piece_cliecked),fieled[i][j]);
            g_signal_connect(fieled[i][j]->button,"leave",G_CALLBACK(piece_leaved),fieled[i][j]);
            g_signal_connect(fieled[i][j]->button,"enter",G_CALLBACK(piece_entered),fieled[i][j]);
        }
    }

}

void reinitialiser_window(Chkswindow *win)
{
    int i,j,k=0,m=0;
    gtk_header_bar_set_title(GTK_HEADER_BAR(win->header_bar),"Checkers Game");
    init_player(win->user,CHKS_OWNER_ONE);
    init_player(win->machine,CHKS_OWNER_TWO);
    for (i=0;i<=7;i++)
    {
        for(j=0;j<=7;j++)
        {
            win->table->filed[i][j]->is_queen=FALSE;
            win->table->filed[i][j]->row=i+1;
            win->table->filed[i][j]->col=j+1;
            if((win->user->Tab[k].x==(i+1)) &&
               (win->user->Tab[k].y==(j+1)))
            {
                win->table->filed[i][j]->owner=CHKS_OWNER_ONE;//user
                k++;
            }else if((win->machine->Tab[m].x==(i+1)) &&
                     (win->machine->Tab[m].y==(j+1)))
            {
                win->table->filed[i][j]->owner=CHKS_OWNER_TWO;//machine
                m++;
            }else win->table->filed[i][j]->owner=CHKS_OWNER_NONE;
        }
    }
    apply_style_button(win->table->filed);
    win->round=1;
}

void Myfonct(GtkWidget *button,Chkswindow *win)
{
    reinitialiser_window(win);
    int c1,c2;
    c1=gtk_combo_box_get_active (GTK_COMBO_BOX(win->combo1));
    c2=gtk_combo_box_get_active (GTK_COMBO_BOX(win->combo2));
    switch (c2) {
        case 0:
            win->depth=2;
            break;
        case 1:
            win->depth=4;
            break;
        case 2:
            win->depth=6;
            break;
    }
    switch (c1) {
        case 1:
            g_timeout_add(1000 * 4, G_SOURCE_FUNC(BotVsBot),win);
    }

}

GtkWidget *Make_headerbar(Chkswindow *win)
{
    GtkWidget *hd;
    GtkWidget *btn;
    hd=gtk_header_bar_new();
    btn= gtk_button_new_with_label("New game");
    g_signal_connect(btn,"clicked",G_CALLBACK(Myfonct),win);
    GtkStyleContext* context = gtk_widget_get_style_context(btn);
    gtk_style_context_add_class(GTK_STYLE_CONTEXT(context),"Newgamebtn");
    GtkWidget *combo1=gtk_combo_box_text_new ();
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo1),"PLayer vs Machine");
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo1),"Machine vs Machine");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo1),0);
    GtkWidget *combo2=gtk_combo_box_text_new ();
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo2),"Easy");
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo2),"Medium");
    gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo2),"Hard");
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo2),0);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(hd),combo1);
    gtk_header_bar_pack_start(GTK_HEADER_BAR(hd),combo2);
    gtk_header_bar_pack_end(GTK_HEADER_BAR(hd),btn);
    gtk_header_bar_set_title(GTK_HEADER_BAR(hd),"Checkers Game");
    gtk_header_bar_set_show_close_button(GTK_HEADER_BAR(hd),TRUE);
    win->combo1=combo1;
    win->combo2=combo2;
    return (GtkWidget*)hd;
}

Chkswindow *make_window(Chkswindow *win)
{
    win->win= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(win->win),600,600);
    gtk_window_move(GTK_WINDOW(win->win),2000,100);
    g_signal_connect(win->win,"destroy",G_CALLBACK(gtk_main_quit),NULL);
    win->g = (Glob*)malloc(sizeof (Glob));
    win->g->f = creer_initaliser_file();
    win->g->field = NULL;
    init_table(win);
    gtk_container_add(GTK_CONTAINER(win->win),win->table->gr);
    win->header_bar = Make_headerbar(win);
    win->round=1;
    gtk_window_set_titlebar(GTK_WINDOW(win->win),win->header_bar);
    g_signal_connect(win->win,"key-press-event",G_CALLBACK(effectuer_moumvment),win->g);
    gtk_widget_show_all(win->win);
}