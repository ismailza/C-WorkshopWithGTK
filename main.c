#include <stdio.h>
#include <gtk/gtk.h>

#define __STYLE__ "D:\\C projects\\Dame_game\\style.css"
typedef enum {
    CHKS_OWNER_NONE,
    CHKS_OWNER_ONE,//user
    CHKS_OWNER_TWO,//machine
} ChksOwner;

#include "sdd.h"

#include "prototyping.h"

#include "allocation_func.h"

#include "macros.h"

#include "help_func.h"

/* ***************************************************************************
| Nom     : delete_value_ChkPlayer; 
| Entrees : ChkPlayer : la structure qui contient les pions restant d'un joueur
|           int       : ligne du pion
|           int       : colone du pion
| Sorties : int       : entier qui indique si la supprission est effectue
| La fonction permet de supprimer un pion pour une joueur
*/
int delete_value_ChkPlayer(ChkPlayer *chpl,int row ,int col)
{
    for(int i=0;chpl->nbr_piece_disp;i++)
    {
        if(chpl->Tab[i].x==row && chpl->Tab[i].y==col)
        {
            for(int j=i;j<chpl->nbr_piece_disp;j++)
            {
                chpl->Tab[j].x=chpl->Tab[j+1].x;
                chpl->Tab[j].y=chpl->Tab[j+1].y;
            }
            chpl->nbr_piece_disp--;
            return (int)1;
        }
    }
    return (int)0;
}
/* ***************************************************************************
| Nom     : verify_for_not_queen;
| Entrees : ChksField :
|           ChksField :
|           int       :
|           int       :
|           File      :
| Sorties : int       :
| 
*/
//row et col sont les coordonnés de last
int verify_for_not_queen(ChksField *filed,ChksField *last,int row,int col,File *f)
{
    int a,b,c,d,droite=0,gauche=0;
    if(f)
    {
        if(f->tete->row==row && f->tete->col==col)defiler(f);
    }
    if(last->view_table->table->filed[row-1][col-1]->row==filed->row &&
       last->view_table->table->filed[row-1][col-1]->col==filed->col)
        return (int)1;
    if(row+1<=8 && col+1<=8)
    {
        if(last->view_table->table->filed[row+1-1][col+1-1]->owner==CHKS_OWNER_TWO)//right
        {
            if(row+2<=8 && col+2<=8)
            {
                if(last->view_table->table->filed[row+2-1][col+2-1]->owner==CHKS_OWNER_NONE) {
                    a = last->view_table->table->filed[row + 2 - 1][col + 2 - 1]->row;
                    b = last->view_table->table->filed[row + 2 - 1][col + 2 - 1]->col;
                    droite = verify_for_not_queen(filed, last, a, b, f);
                }
            }
        }
    }
    if(row+1<=8 && col-1>=1)
    {
        if(last->view_table->table->filed[row+1-1][col-1-1]->owner==CHKS_OWNER_TWO)//left
        {
            if(row+2<=8 && col-2>=1)
            {
                if(last->view_table->table->filed[row+2-1][col-2-1]->owner==CHKS_OWNER_NONE)
                {
                    a=last->view_table->table->filed[row+2-1][col-2-1]->row;
                    b=last->view_table->table->filed[row+2-1][col-2-1]->col;
                    gauche=verify_for_not_queen(filed,last,a,b,f);
                }
            }
        }
    }
    if(droite || gauche)return (int)1;
    else return (int)0;
}
/* ***************************************************************************
| Nom     : first_is_one_four;
| Entrees : ChksField :
| Sorties : int				:
| 
*/
int first_is_one_four(ChksField *last)
{
    int a,b;
    a=last->row;
    b=last->col;
    if((a+1<=8) && (a-1>=1) && (b+1<=8) && (b-1>=1))
    {
        if((last->view_table->table->filed[a+1-1][b+1-1]->owner==CHKS_OWNER_ONE) &&
           (last->view_table->table->filed[a+1-1][b-1-1]->owner==CHKS_OWNER_ONE) &&
           (last->view_table->table->filed[a-1-1][b-1-1]->owner==CHKS_OWNER_ONE) &&
           (last->view_table->table->filed[a-1-1][b+1-1]->owner==CHKS_OWNER_ONE ))return (int)1;
        else return (int)0;
    }else  return (int)0;

}
/* ***************************************************************************
| Nom     : first_is_one;
| Entrees : ChksField :
| 				: int				:
| Sorties : int				:
| 
*/
int first_is_one(ChksField *last,int i)
{
    int a,b;
    a=last->row;
    b=last->col;
    switch (i) {
        case 1:
            if(a-1>=1 && b+1<=8)
            {
                if(last->view_table->table->filed[a-1-1][b+1-1]->owner==CHKS_OWNER_ONE)return (int)1;
                else return (int)0;
            }else return (int)0;
        case 2:
            if(a-1>=1 && b-1>=1)
            {
                if(last->view_table->table->filed[a-1-1][b-1-1]->owner==CHKS_OWNER_ONE)return (int)1;
                else return (int)0;
            }else return (int)0;
        case 3:
            if(a+1<=8 && b-1>=1)
            {
                if(last->view_table->table->filed[a+1-1][b-1-1]->owner==CHKS_OWNER_ONE)return (int)1;
                else return (int)0;
            }else return (int)0;
        case 4:
            if(a+1<=8 && b+1<=8)
            {
                if(last->view_table->table->filed[a+1-1][b+1-1]->owner==CHKS_OWNER_ONE)return (int)1;
                else return (int)0;
            }else return (int)0;
    }
    return (int)0;
}
/* ***************************************************************************
| Nom     : first_one_or_two;
| Entrees : ChksField *:
| 				: int				*:
| 				: int				*:
| 				: int				 :
| 				: ChksOwner  :
| 				: etat_window *:
|					: int 			 :
|					: int 			 :
| Sorties : int				 :
| 
*/
void first_one_or_two(ChksField *last,int *a,int *b,int i,ChksOwner owner,etat_window *etw,int row,int col)
{
    int k,j;
    if(last)
    {
        k=last->row;
        j=last->col;
    }else
    {
        k=row;
        j=col;
    }
    switch (i) {
        case 1:
            k--;
            j++;
            while (k>=1 && j<=8)
            {
                if(last)
                {
                    if(last->view_table->table->filed[k-1][j-1]->owner==owner)
                    {
                        *a=last->view_table->table->filed[k-1][j-1]->row;
                        *b=last->view_table->table->filed[k-1][j-1]->col;
                        return;
                    }else if(last->view_table->table->filed[k-1][j-1]->owner==
                    CHKS_OWNER_NONE)
                    {
                        k--;
                        j++;
                        continue;
                    }else break;
                }else
                {
                    if(etw->filed[k-1][j-1]->owner==owner)
                    {
                        *a=etw->filed[k-1][j-1]->row;
                        *b=etw->filed[k-1][j-1]->col;
                        return;
                    }else if(etw->filed[k-1][j-1]->owner==
                             CHKS_OWNER_NONE)
                    {
                        k--;
                        j++;
                        continue;
                    }else break;
                }
            }
            break;
        case 2:
            k--;
            j--;
            while (k>=1 && j>=1)
            {
                if(last)
                {
                    if(last->view_table->table->filed[k-1][j-1]->owner==owner)
                    {
                        *a=last->view_table->table->filed[k-1][j-1]->row;
                        *b=last->view_table->table->filed[k-1][j-1]->col;
                        return;
                    }else if(last->view_table->table->filed[k-1][j-1]->owner==
                             CHKS_OWNER_NONE)
                    {
                        k--;
                        j--;
                        continue;
                    }else break;
                }else
                {
                    if(etw->filed[k-1][j-1]->owner==owner)
                    {
                        *a=etw->filed[k-1][j-1]->row;
                        *b=etw->filed[k-1][j-1]->col;
                        return;
                    }else if(etw->filed[k-1][j-1]->owner==
                             CHKS_OWNER_NONE)
                    {
                        k--;
                        j--;
                        continue;
                    }else break;
                }
            }
            break;
        case 3:
            k++;
            j--;
            while (k<=8 && j>=1)
            {
                if(last)
                {
                    if(last->view_table->table->filed[k-1][j-1]->owner==owner)
                    {
                        *a=last->view_table->table->filed[k-1][j-1]->row;
                        *b=last->view_table->table->filed[k-1][j-1]->col;
                        return;
                    }else if(last->view_table->table->filed[k-1][j-1]->owner==
                             CHKS_OWNER_NONE)
                    {
                        k++;
                        j--;
                        continue;
                    }else break;
                }else{
                    if(etw->filed[k-1][j-1]->owner==owner)
                    {
                        *a=etw->filed[k-1][j-1]->row;
                        *b=etw->filed[k-1][j-1]->col;
                        return;
                    }else if(etw->filed[k-1][j-1]->owner==
                             CHKS_OWNER_NONE)
                    {
                        k++;
                        j--;
                        continue;
                    }else break;
                }
            }
            break;
        case 4:
            k++;
            j++;
            while (k<=8 && j<=8)
            {
                if(last)
                {
                    if(last->view_table->table->filed[k-1][j-1]->owner==owner)
                    {
                        *a=last->view_table->table->filed[k-1][j-1]->row;
                        *b=last->view_table->table->filed[k-1][j-1]->col;
                        return;
                    }else if(last->view_table->table->filed[k-1][j-1]->owner==
                             CHKS_OWNER_NONE)
                    {
                        k++;
                        j++;
                        continue;
                    }else break;
                }else
                {
                    if(etw->filed[k-1][j-1]->owner==owner)
                    {
                        *a=etw->filed[k-1][j-1]->row;
                        *b=etw->filed[k-1][j-1]->col;
                        return;
                    }else if(etw->filed[k-1][j-1]->owner==
                             CHKS_OWNER_NONE)
                    {
                        k++;
                        j++;
                        continue;
                    }else break;
                }
            }
            break;
    }
    *a=-1;
    *b=-1;

}
/* ***************************************************************************
| Nom     : two_consecutif;
| Entrees : int			   : 
| 				: int				 :
| 				: ChksField *:
| 				: int				 :
| 				: ChksOwner  :
| Sorties : int				 :
| 
*/
int two_consecutif(int row,int col ,ChksField *last,int i,ChksOwner owner)
{
    switch (i) {
        case 1:
            if(last->view_table->table->filed[row-1][col-1]->owner==owner)
            {
                if(((row-1)>=1 )&& ((col+1)<=8))
                {
                    if(last->view_table->table->filed[row-1-1][col+1-1]->owner==owner)return (int)1;
                    else return (int)0;
                }else return (int)0;

            }else return (int)0;
            break;
        case 2:
            if(last->view_table->table->filed[row-1][col-1]->owner==owner)
            {
                if(((row-1)>=1 )&& ((col-1)>=1))
                {
                    if(last->view_table->table->filed[row-1-1][col-1-1]->owner==owner)return (int)1;
                    else return (int)0;
                }else return (int)0;

            }else return (int)0;
            break;
        case 3:
            if(last->view_table->table->filed[row-1][col-1]->owner==owner)
            {
                if(((row+1)<=8)&& ((col-1)>=1))
                {
                    if(last->view_table->table->filed[row+1-1][col-1-1]->owner==owner)return (int)1;
                    else return (int)0;
                }else return (int)0;
            }else return (int)0;
            break;
        case 4:
            if(last->view_table->table->filed[row-1][col-1]->owner==owner)
            {
                if(((row+1)<=8)&& ((col+1)<=8))
                {
                    if(last->view_table->table->filed[row+1-1][col+1-1]->owner==owner)return (int)1;
                    else return (int)0;
                }else return (int)0;
            }else return (int)0;
            break;
    }
}
/* ***************************************************************************
| Nom     : two_consecutif2;
| Entrees : int			    : 
| 				: int				  :
| 				: etat_window *:
| 				: int				  :
| 				: ChksOwner   :
| Sorties : int				  :
| 
*/
int two_consecutif2(int row,int col ,etat_window *etw,int i,ChksOwner owner)
{
    switch (i) {
        case 1:
            if(etw->filed[row-1][col-1]->owner==owner)
            {
                if(((row-1)>=1 )&& ((col+1)<=8))
                {
                    if(etw->filed[row-1-1][col+1-1]->owner==owner)return (int)1;
                    else return (int)0;
                }else return (int)0;
            }else return (int)0;
            break;
        case 2:
            if(etw->filed[row-1][col-1]->owner==owner)
            {
                if(((row-1)>=1 )&& ((col-1)>=1))
                {
                    if(etw->filed[row-1-1][col-1-1]->owner==owner)return (int)1;
                    else return (int)0;
                }else return (int)0;

            }else return (int)0;
            break;
        case 3:
            if(etw->filed[row-1][col-1]->owner==owner)
            {
                if(((row+1)<=8)&& ((col-1)>=1))
                {
                    if(etw->filed[row+1-1][col-1-1]->owner==owner)return (int)1;
                    else return (int)0;
                }else return (int)0;
            }else return (int)0;
            break;
        case 4:
            if(etw->filed[row-1][col-1]->owner==owner)
            {
                if(((row+1)<=8)&& ((col+1)<=8))
                {
                    if(etw->filed[row+1-1][col+1-1]->owner==owner)return (int)1;
                    else return (int)0;
                }else return (int)0;
            }else return (int)0;
            break;
    }
}

int exist_case_beetwen_one_two(ChksField *last,ChksField *field,int i,File *f)
{
    int c,d,cpt=0,kk=0;
    switch (i) {
        case 1:
            c=last->row-1;
            d=last->col+1;
            if(c<1 || d>8)return (int)-2;
            do {
                //le champs est le correspond
                if((field->view_table->table->filed[c-1][d-1]->row==field->row) &&
                   (field->view_table->table->filed[c-1][d-1]->col==field->col) &&
                        (field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_NONE))
                    return (int)1;
                else if(field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_TWO && cpt==0)cpt=1;
                else if((cpt==1) && (field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_NONE))
                {
                    cellule *ne= creer_initialiser_cellule(c,d,3,i);
                    enfiler(f,ne);
                    kk++;
                }
                if(two_consecutif(c,d,last,i,CHKS_OWNER_TWO) ||
                   field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_ONE)
                    break;
                c--;
                d++;
            } while (c>=1 && d<=8);
            if(kk==0)return (int)-2;//on a rien ajouter a la file pour la traiter
            else return (int)-3;
        case 2:
            c=last->row-1;
            d=last->col-1;
            if(c<1 || d<1 )return (int)-2;
            do {
                //le champs est le correspond
                if((field->view_table->table->filed[c-1][d-1]->row==field->row) &&
                   (field->view_table->table->filed[c-1][d-1]->col==field->col) &&
                   (field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_NONE))
                    return (int)1;
                else if(field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_TWO && cpt==0)cpt=1;
                else if((cpt==1) && (field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_NONE))
                {
                    cellule *ne= creer_initialiser_cellule(c,d,4,i);
                    enfiler(f,ne);
                    kk++;
                }
                if(two_consecutif(c,d,last,i,CHKS_OWNER_TWO) ||
                field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_ONE)
                    break;
                c--;
                d--;
            } while (c>=1 && d>=1);
            if(kk==0)return (int)-2;//on a rien ajouter a la file pour la traiter
            else return (int)-3;
        case 3:
            c=last->row+1;
            d=last->col-1;
            if(c>8 || d<1)return (int)-2;
            do {
                //le champs est le correspond
                if((field->view_table->table->filed[c-1][d-1]->row==field->row) &&
                   (field->view_table->table->filed[c-1][d-1]->col==field->col) &&
                   (field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_NONE))
                    return (int)1;
                else if(field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_TWO && cpt==0)cpt=1;
                else if((cpt==1) && (field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_NONE))
                {
                    cellule *ne= creer_initialiser_cellule(c,d,1,i);
                    enfiler(f,ne);
                    kk++;
                }
                if(two_consecutif(c,d,last,i,CHKS_OWNER_TWO) ||
                   field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_ONE)
                    break;
                c++;
                d--;
            } while (c<=8 && d>=1);
            if(kk==0)return (int)-2;//on a rien ajouter a la file pour la traiter
            else return (int)-3;
        case 4:
            c=last->row+1;
            d=last->col+1;
            if((c>8) || (d>8))
                return (int)-2;
            do {
                //le champs est le correspond
                if((field->view_table->table->filed[c-1][d-1]->row==field->row) &&
                   (field->view_table->table->filed[c-1][d-1]->col==field->col) &&
                   (field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_NONE))
                    return (int)1;
                else if(field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_TWO && cpt==0)cpt=1;
                else if((cpt==1) && (field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_NONE))
                {
                    cellule *ne= creer_initialiser_cellule(c,d,2,i);
                    enfiler(f,ne);
                    kk++;
                }
                if(two_consecutif(c,d,last,i,CHKS_OWNER_TWO) ||
                   field->view_table->table->filed[c-1][d-1]->owner==CHKS_OWNER_ONE)
                    break;
                c++;
                d++;
            } while (c<=8 && d<=8);
            if(kk==0)return (int)-2;//on a rien ajouter a la file pour la traiter
            else return (int)-3;
    }
}

int techek_entred_piece(ChksField *filed,ChksField *last,File *f1)
{
    int i,er=0,m,c,d;
    ChksField *ff;
    //si le champ survoler est different de none rien faire
    if(filed->owner==CHKS_OWNER_TWO || filed->owner==CHKS_OWNER_ONE)return (int)0;
    //si le diagonal en bas a gauche et a droite contient piece de l'utilisateur
    if(!last->is_queen)
    {
        if(last->row+1<=8 && last->col-1>=1 && last->col+1<=8)
        {
            if(last->view_table->table->filed[last->row+1-1][last->col-1-1]->owner==CHKS_OWNER_ONE &&
               last->view_table->table->filed[last->row+1-1][last->col+1-1]->owner==CHKS_OWNER_ONE)
                return (int)0;
        }
        if(filed->row==last->row+1 && filed->col==last->col+1)return (int)1;
        if(filed->row==last->row+1 && filed->col==last->col-1)return (int)1;
        return (int)verify_for_not_queen(filed,last,last->row,last->col,f1);
    }else{//verification pour queen
        /*
        * j'ai trouvé 4 champs one
        * il ne peut pas ce deplacer
        */
        if(first_is_one_four(last))return (int)0;
        File *f=creer_initaliser_file();
        if(est_vide_file(f))
        {
           for(i=1;i<=4;i++)
           {
               //Dans cette direction field one ne peut pas ce deplacer
               if(first_is_one(last,i))
               {
                   er++;//on a rien inserer dans la file
                   continue;
               }
               //verifier si il existe l'adversaire dans cette direction
               first_one_or_two(last,&c,&d,i,CHKS_OWNER_TWO,NULL,-1,-1);
               m=exist_case_beetwen_one_two(last,filed,i,f);
               if(m==-2)er++;
               else if(m==1)
               {
                   if(f1)
                   {
                       if(filed->row==f1->tete->row &&
                               filed->col==f1->tete->col)
                           defiler(f1);
                   }
                   return (int)1;
               }
           }
        }
        if(er==4)return (int)0;/*il n'a pas trouvé et il n'existe pas une case beetwen last machine field
         and first one field   */
        while (!est_vide_file(f))
        {
            if(f1)
            {
                if(f->tete->row==f1->tete->row &&
                   f->tete->col==f1->tete->col)
                    defiler(f1);
            }
            for(i=1;i<=4;i++)
            {
                if(i!=f->tete->diag_not_verify && i!=f->tete->diag_not_verify2)
                {
                    ff=last->view_table->table->filed[f->tete->row-1][f->tete->col-1];
                    first_one_or_two(ff,&c,&d,i,CHKS_OWNER_TWO,NULL,-1,-1);
                    if((c==-1)&&(d==-1))continue;
                    ff=last->view_table->table->filed[c-1][d-1];
                    m=exist_case_beetwen_one_two(ff,filed,i,f);
                    if(m==1)return (int)1;//field trouvé
                }
            }
            defiler(f);
        }
        return (int)0;
    }
}

cellule2 *construire_arbre1(etat_window *etw,cellule2 *ar,int row,int col)//machine
{
    if((row-1-1>=0) && (col+1-1<=7 ) && (row-2-1>=0) && (col+2-1<=7))
    {
        if((etw->filed[row-1-1][col+1-1]->owner==CHKS_OWNER_ONE) &&
           (etw->filed[row-2-1][col+2-1]->owner==CHKS_OWNER_NONE))
        {
            cellule2 *ne= crrer_initailiser_ccellule2(0,row-2,col+2);
            ar->topright=construire_arbre1(etw,ne,row-2,col+2);
        }
    }
    if((row-1-1>=0) && (col-1-1>=0 ) && (row-2-1>=0) && (col-2-1>=0))
    {
        if((etw->filed[row-1-1][col-1-1]->owner==CHKS_OWNER_ONE) &&
           (etw->filed[row-2-1][col-2-1]->owner==CHKS_OWNER_NONE))
        {
            cellule2 *ne= crrer_initailiser_ccellule2(0,row-2,col-2);
            ar->topleft=construire_arbre1(etw,ne,row-2,col-2);
        }
    }
    return (cellule2*)ar;
}

cellule2 *construire_arbre2(etat_window *etw,cellule2 *ar,int row,int col)//user
{

    if((row+1-1<=7) && (col+1-1<=7 ) && (row+2-1<=7) && (col+2-1<=7))
    {
        if((etw->filed[row+1-1][col+1-1]->owner==CHKS_OWNER_TWO) &&
           (etw->filed[row+2-1][col+2-1]->owner==CHKS_OWNER_NONE))
        {
            cellule2 *ne= crrer_initailiser_ccellule2(0,row+2,col+2);
            ar->topright=construire_arbre2(etw,ne,row+2,col+2);
        }
    }
    if((row+1-1<=7) && (col-1-1>=0 ) && (row+2-1<=7) && (col-2-1>=0))
    {
        if((etw->filed[row+1-1][col-1-1]->owner==CHKS_OWNER_TWO) &&
           (etw->filed[row+2-1][col-2-1]->owner==CHKS_OWNER_NONE))
        {
            cellule2 *ne= crrer_initailiser_ccellule2(0,row+2,col-2);
            ar->topleft=construire_arbre2(etw,ne,row+2,col-2);
        }
    }
    return (cellule2*)ar;
}

cellule2 *add_none_first(cellule2 *list,int i,etat_window *etw,int row,int col)
{

    switch (i) {
        case 1:
            row=row-1;
            col=col+1;
            while (row>=1 &&col<=8)
            {
                if(etw->filed[row-1][col-1]->owner==CHKS_OWNER_NONE)
                {
                    list=inserer_fin_liste_cellule2(list,0,row,col);
                    row--;
                    col++;
                }else break;
            }
            break;
        case 2:
            row=row-1;
            col=col-1;
            while (row>=1 &&col>=1)
            {
                if(etw->filed[row-1][col-1]->owner==CHKS_OWNER_NONE)
                {
                    list=inserer_fin_liste_cellule2(list,0,row,col);
                    row--;
                    col--;
                }else break;
            }
            break;

        case 3:
            row=row+1;
            col=col-1;
            while (row<=8 &&col>=1)
            {
                if(etw->filed[row-1][col-1]->owner==CHKS_OWNER_NONE)
                {
                    list=inserer_fin_liste_cellule2(list,0,row,col);
                    row++;
                    col--;
                }else break;
            }
            break;
        case 4:
            row=row+1;
            col=col+1;
            while (row<=8 &&col<=8)
            {
                if(etw->filed[row-1][col-1]->owner==CHKS_OWNER_NONE)
                {
                    list=inserer_fin_liste_cellule2(list,0,row,col);
                    row++;
                    col++;
                }else break;
            }
            break;
    }
    return (cellule2*)list;
}

cellule2 *construire_arbre_queen(ChksField *field,cellule2 *list,int onerow,int onecol,int i)
{
    int c,d,k,l;
    cellule2 *kk=NULL;
    int onerow1,onecol1,tworow1,twocol1;
    ChksField *ff=field->view_table->table->filed[onerow-1][onecol-1];
    if(i!=4)//for 2
    {
        first_one_or_two(ff,&c,&d,2,CHKS_OWNER_ONE,NULL,-1,-1);
        if(c!=-1 && d!=-1)
        {
            first_one_or_two(ff,&k,&l,2,CHKS_OWNER_TWO,NULL,-1,-1);
            onerow1=c-1;
            onecol1=d-1;
            tworow1=k+1;
            twocol1=l+1;
            while (onerow1>=tworow1 && onecol1 >=twocol1)
            {
                if(field->view_table->table->filed[onerow1-1][onecol1-1]->owner==CHKS_OWNER_NONE)
                {
                    kk=construire_arbre_queen(field,list->topleft,onerow1,onecol1,4);
                    list->topleft= inserer_debut(list->topleft,kk);
                }
                onerow1--;
                onecol1--;
            }
        }
    }
    if(i!=2)//for 4
    {
        first_one_or_two(ff,&c,&d,4,CHKS_OWNER_ONE,NULL,-1,-1);
        if(c!=-1 && d!=-1)
        {
            first_one_or_two(ff,&k,&l,4,CHKS_OWNER_TWO,NULL,-1,-1);
            onerow1=c+1;
            onecol1=d+1;
            tworow1=k-1;
            twocol1=l-1;
            while (onerow1<=tworow1 && onecol1 <=twocol1)
            {
                if(field->view_table->table->filed[onerow1-1][onecol1-1]->owner==CHKS_OWNER_NONE)
                {
                    kk=construire_arbre_queen(field,list->butright,onerow1,onecol1,4);
                    list->butright= inserer_debut(list->butright,kk);
                }
                onerow1++;
                onecol1++;
            }
        }
    }
    if(i!=1)//for 3
    {
        first_one_or_two(ff,&c,&d,3,CHKS_OWNER_ONE,NULL,-1,-1);
        if(c!=-1 && d!=-1)
        {
            first_one_or_two(ff,&k,&l,3,CHKS_OWNER_TWO,NULL,-1,-1);
            onerow1=c+1;
            onecol1=d-1;
            tworow1=k-1;
            twocol1=l+1;
            while (onerow1<=tworow1 && onecol1 >=twocol1)
            {
                if(field->view_table->table->filed[onerow1-1][onecol1-1]->owner==CHKS_OWNER_NONE)
                {
                    kk=construire_arbre_queen(field,list->butleft,onerow1,onecol1,4);
                    list->butleft= inserer_debut(list->butleft,kk);
                }
                onerow1++;
                onecol1--;
            }
        }
    }
    if(i!=3)//for 1
    {
        first_one_or_two(ff,&c,&d,1,CHKS_OWNER_ONE,NULL,-1,-1);
        if(c!=-1 && d!=-1)
        {
            first_one_or_two(ff,&k,&l,1,CHKS_OWNER_TWO,NULL,-1,-1);
            onerow1=c-1;
            onecol1=d+1;
            tworow1=k+1;
            twocol1=l-1;
            while (onerow1>=tworow1 && onecol1 <=twocol1)
            {
                if(field->view_table->table->filed[onerow1-1][onecol1-1]->owner==CHKS_OWNER_NONE)
                {
                    kk=construire_arbre_queen(field,list->topright,onerow1,onecol1,4);
                    list->topright= inserer_debut(list->topright,kk);
                }
                onerow1--;
                onecol1++;
            }
        }
    }
    return (cellule2*)list;
}

void drop(ChksField *last,ChksField *field,ChksOwner owner)
{
    int lrow,lcol,frow,fcol,a=-1,b=-1;
    GtkStyleContext* context;
    lrow=last->row;
    lcol=last->col;
    frow=field->row;
    fcol=field->col;
    if(frow > lrow && fcol < lcol)
    {
        while (frow>lrow && fcol <lcol)
        {
            if(field->view_table->table->filed[frow-1][fcol-1]->owner==owner)
            {
                field->view_table->table->filed[frow-1][fcol-1]->owner=CHKS_OWNER_NONE;
                a=frow;
                b=fcol;
            }
            frow--;
            fcol++;
        }
    }else if(frow> lrow && fcol > lcol)
    {
        while (frow>lrow && fcol >lcol)
        {
            if(field->view_table->table->filed[frow-1][fcol-1]->owner==owner)
            {
                field->view_table->table->filed[frow-1][fcol-1]->owner=CHKS_OWNER_NONE;
                a=frow;
                b=fcol;
            }
            frow--;
            fcol--;
        }
    }else if(frow< lrow && fcol > lcol)
    {
        while (frow<lrow && fcol >lcol)
        {
            if(field->view_table->table->filed[frow-1][fcol-1]->owner==owner)
            {
                field->view_table->table->filed[frow-1][fcol-1]->owner=CHKS_OWNER_NONE;
                a=frow;
                b=fcol;
            }
            frow++;
            fcol--;
        }
    }else if(frow < lrow && fcol < lcol)
    {
        while (frow <lrow && fcol <lcol)
        {
            if(field->view_table->table->filed[frow-1][fcol-1]->owner==owner)
            {
                field->view_table->table->filed[frow-1][fcol-1]->owner=CHKS_OWNER_NONE;
                a=frow;
                b=fcol;
            }
            frow++;
            fcol++;
        }
    }
    if(a!=-1 && b!=-1)
    {
        switch (owner) {
            case CHKS_OWNER_ONE:
                delete_value_ChkPlayer(field->view_table->user,a,b);
                break;
            case CHKS_OWNER_TWO:
                delete_value_ChkPlayer(field->view_table->machine,a,b);
                break;
        }
    }
}

void drop1(etat_window *etw1,ChksOwner owner,int lrow,int lcol,int frow,int fcol)
{
    GtkStyleContext* context;
    int a=-1,b=-1;
    if(frow > lrow && fcol < lcol)
    {
        while (frow>lrow && fcol <lcol)
        {
            if(etw1->filed[frow-1][fcol-1]->owner==owner)
            {
                etw1->filed[frow-1][fcol-1]->owner=CHKS_OWNER_NONE;
                a=frow;
                b=fcol;
            }
            frow--;
            fcol++;
        }
    }else if(frow> lrow && fcol > lcol)
    {
        while (frow>lrow && fcol >lcol)
        {
            if(etw1->filed[frow-1][fcol-1]->owner==owner)
            {
                etw1->filed[frow-1][fcol-1]->owner=CHKS_OWNER_NONE;
                a=frow;
                b=fcol;
            }
            frow--;
            fcol--;
        }
    }else if(frow< lrow && fcol > lcol)
    {
        while (frow<lrow && fcol >lcol)
        {
            if(etw1->filed[frow-1][fcol-1]->owner==owner)
            {
                etw1->filed[frow-1][fcol-1]->owner=CHKS_OWNER_NONE;
                a=frow;
                b=fcol;
            }
            frow++;
            fcol--;
        }
    }else if(frow < lrow && fcol < lcol)
    {
        while (frow <lrow && fcol <lcol)
        {
            if(etw1->filed[frow-1][fcol-1]->owner==owner)
            {
                etw1->filed[frow-1][fcol-1]->owner=CHKS_OWNER_NONE;
                a=frow;
                b=fcol;
            }
            frow++;
            fcol++;
        }
    }
    if(a!=-1 && b!=-1)
    {
        switch (owner) {
            case CHKS_OWNER_ONE:
                delete_value_ChkPlayer(etw1->user,a,b);
                break;
            case CHKS_OWNER_TWO:
                delete_value_ChkPlayer(etw1->machine,a,b);
                break;
        }
    }
}

//file sera rempli si il ya plusieurs deplacement
void drop_two_betwen_fl_lt(ChksField *last,ChksField *field,File *f,ChksOwner owner)
{
    int a,b,c,d;
    if(!f)
    {
        drop(last,field,owner);
    }else{
        defiler(f);
        a=f->tete->row;
        b=f->tete->col;
        drop(last,field->view_table->table->filed[a-1][b-1],owner);
        defiler(f);
        if(est_vide_file(f))return;
        do
        {
            c=f->tete->row;
            d=f->tete->col;
            drop(field->view_table->table->filed[a-1][b-1],
                 field->view_table->table->filed[c-1][d-1],owner);
            defiler(f);
        } while (!est_vide_file(f));
    }
}

void drop_two_betwen_fl_lt1(etat_window *etw1,File *f,ChksOwner owner,int lrow,int lcol,int frow,int fcol)
{
    int a,b,c,d;
    if(!f)
    {
        drop1(etw1,owner,lrow,lcol,frow,fcol);
    }else{
        defiler(f);
        a=f->tete->row;
        b=f->tete->col;
        drop1(etw1,owner,lrow,lcol,a,b);
        defiler(f);
        if(est_vide_file(f))return;
        do
        {
            c=f->tete->row;
            d=f->tete->col;
            drop1(etw1,owner,a,b,c,d);
            defiler(f);
        } while (!est_vide_file(f));
    }
}

void copie_win_field(Chkswindow *winS,etat_window *winD)
{
    int i,j;
    for(i=0;i<winS->user->nbr_piece_disp;i++)
    {
        winD->user->Tab[i].x=winS->user ->Tab[i].x;
        winD->user->Tab[i].y=winS->user ->Tab[i].y;
    }
    winD->user->nbr_piece_disp=winS->user->nbr_piece_disp;
    for(i=0;i<winS->machine->nbr_piece_disp;i++)
    {
        winD->machine->Tab[i].x=winS->machine ->Tab[i].x;
        winD->machine->Tab[i].y=winS->machine ->Tab[i].y;
    }
    winD->machine->nbr_piece_disp=winS->machine->nbr_piece_disp;
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            winD->filed[i][j]->row=winS->table->filed[i][j]->row;
            winD->filed[i][j]->col=winS->table->filed[i][j]->col;
            winD->filed[i][j]->owner=winS->table->filed[i][j]->owner;
            winD->filed[i][j]->is_queen=winS->table->filed[i][j]->is_queen;
        }
    }
}

void copie_win_field1(etat_window *winS,etat_window *winD)
{
    int i,j;
    for(i=0;i<winS->user->nbr_piece_disp;i++)
    {
        winD->user->Tab[i].x=winS->user ->Tab[i].x;
        winD->user->Tab[i].y=winS->user ->Tab[i].y;
    }
    winD->user->nbr_piece_disp=winS->user->nbr_piece_disp;
    for(i=0;i<winS->machine->nbr_piece_disp;i++)
    {
        winD->machine->Tab[i].x=winS->machine ->Tab[i].x;
        winD->machine->Tab[i].y=winS->machine ->Tab[i].y;
    }
    winD->machine->nbr_piece_disp=winS->machine->nbr_piece_disp;
    for(i=0;i<8;i++)
    {
        for(j=0;j<8;j++)
        {
            winD->filed[i][j]->row=winS->filed[i][j]->row;
            winD->filed[i][j]->col=winS->filed[i][j]->col;
            winD->filed[i][j]->owner=winS->filed[i][j]->owner;
            winD->filed[i][j]->is_queen=winS->filed[i][j]->is_queen;
        }
    }
}

cellule2 *queen_arbre1(cellule2 *ar,int i,etat_window *etw,int onerow,int onecol,ChksOwner owner,ChksOwner notowner)//machine
{
    int c,d,f,g;
    ChksField *field1;
    switch (i)
    {
        case 1://top right
            f=onerow;
            g=onecol;
            do{
                field1=etw->filed[onerow-1][onecol-1];
                if(field1->owner==CHKS_OWNER_NONE)
                {
                    etw->filed[f-1][g-1]->owner=CHKS_OWNER_NONE;
                    cellule2 *ne= crrer_initailiser_ccellule2(0,onerow,onecol);
                    first_one_or_two(NULL,&c,&d,2,owner,etw,field1->row,field1->col);
                    if(c!=-1 && d!=-1)
                    {

                        ne=queen_arbre1(ne,2,etw,c,d,owner,notowner);
                    }
                    first_one_or_two(NULL,&c,&d,4,owner,etw,field1->row,field1->col);
                    if(c!=-1 && d!=-1)
                    {
                        ne=queen_arbre1(ne,4,etw,c,d,owner,notowner);
                    }
                    ar->topright=inserer_debut(ar->topright,ne);
                }else if((field1->owner==notowner)||two_consecutif2(onerow,onecol,etw,i,owner))break;
                onerow--;
                onecol++;
            } while ((onerow>=1) && (onecol<=8));
            break;
        case 2://top left
            f=onerow;
            g=onecol;
            do{
                field1=etw->filed[onerow-1][onecol-1];
                if(field1->owner==CHKS_OWNER_NONE)
                {
                    etw->filed[f-1][g-1]->owner=CHKS_OWNER_NONE;
                    cellule2 *ne= crrer_initailiser_ccellule2(0,onerow,onecol);
                    first_one_or_two(NULL,&c,&d,1,owner,etw,field1->row,field1->col);
                    if(c!=-1 && d!=-1)
                    {
                        ne=queen_arbre1(ne,1,etw,c,d,owner,notowner);
                    }
                    first_one_or_two(NULL,&c,&d,3,owner,etw,field1->row,field1->col);
                    if(c!=-1 && d!=-1)
                    {
                        ne=queen_arbre1(ne,3,etw,c,d,owner,notowner);
                    }
                    ar->topleft=inserer_debut(ar->topleft,ne);
                }else if((field1->owner==notowner) || (two_consecutif2(onerow,onecol,etw,i,owner)))break;
                onerow--;
                onecol--;
            } while ((onerow>=1) && (onecol>=1));
            break;
        case 3://buttom left
            f=onerow;
            g=onecol;
            do{
                field1=etw->filed[onerow-1][onecol-1];
                if(field1->owner==CHKS_OWNER_NONE)
                {
                    etw->filed[f-1][g-1]->owner=CHKS_OWNER_NONE;
                    cellule2 *ne= crrer_initailiser_ccellule2(0,onerow,onecol);
                    first_one_or_two(NULL,&c,&d,2,owner,etw,field1->row,field1->col);
                    if(c!=-1 && d!=-1)
                    {
                        ne=queen_arbre1(ne,2,etw,c,d,owner,notowner);
                    }
                    first_one_or_two(NULL,&c,&d,4,owner,etw,field1->row,field1->col);
                    if(c!=-1 && d!=-1)
                    {
                        ne=queen_arbre1(ne,4,etw,c,d,owner,notowner);
                    }
                    ar->butleft=inserer_debut(ar->butleft,ne);
                }else if((field1->owner==notowner) || (two_consecutif2(onerow,onecol,etw,i,owner)))break;
                onerow++;
                onecol--;
            } while ((onerow<=8) && (onecol>=1));
            break;
        case 4://buttom right
            f=onerow;
            g=onecol;
            do{
                field1=etw->filed[onerow-1][onecol-1];
                if(field1->owner==CHKS_OWNER_NONE)
                {
                    etw->filed[f-1][g-1]->owner=CHKS_OWNER_NONE;
                    cellule2 *ne= crrer_initailiser_ccellule2(0,onerow,onecol);
                    first_one_or_two(NULL,&c,&d,1,owner,etw,field1->row,field1->col);
                    if(c!=-1 && d!=-1)
                    {
                        ne=queen_arbre1(ne,1,etw,c,d,owner,notowner);
                    }
                    first_one_or_two(NULL,&c,&d,3,owner,etw,field1->row,field1->col);
                    if(c!=-1 && d!=-1)
                    {
                        ne=queen_arbre1(ne,3,etw,c,d,owner,notowner);
                    }
                    ar->butright=inserer_debut(ar->butright,ne);
                }else if((field1->owner==notowner) || (two_consecutif2(onerow,onecol,etw,i,owner)))break;
                onerow++;
                onecol++;
            } while ((onerow<=8) && (onecol<=8));
            break;
    }
    return (cellule2*)ar;
}

cellule2 **table_arbre_shemin(cellule2 **cl,etat_window *etw,ChksOwner owner)
{
    int taille,n,i,row,col,j;
    ChksField *field;
    switch (owner) {
        case CHKS_OWNER_ONE:
            taille=etw->user->nbr_piece_disp;
            cl=(cellule2**) malloc(sizeof (cellule2*)*taille);
            if(!cl)
            {
                printf("error allocation");
                exit(-1);
            }
            for(n=0;n<taille;n++)cl[n]=NULL;
            for(i=0;i<taille;i++)
            {
                row=etw->user->Tab[i].x;
                col=etw->user->Tab[i].y;
                if(!etw->filed[row-1][col-1]->is_queen)
                {
                    if((row+1-1<=7) && (col-1-1>=0))
                    {
                        if(etw->filed[row+1-1][col-1-1]->owner==CHKS_OWNER_NONE)//left
                            cl[i]=inserer_fin_liste_cellule2(cl[i],0,row+1,col-1);
                    }
                    if((row+1-1<=7) && (col+1-1<=7))
                    {
                        if(etw->filed[row+1-1][col+1-1]->owner==CHKS_OWNER_NONE)//right
                            cl[i]=inserer_fin_liste_cellule2(cl[i],0,row+1,col+1);
                    }
                    cellule2 *ne_test= crrer_initailiser_ccellule2(0,row,col);
                    ne_test=construire_arbre2(etw,ne_test,row,col);
                    if(ne_test->topleft || ne_test->topright)
                    {
                        if(!cl[i])
                            cl[i]=ne_test;
                        else {
                            ne_test->suiv = cl[i];
                            cl[i] = ne_test;
                        }
                    }
                }else//  if queen ,a traité
                {
                    cellule2 *aa=NULL;
                    cellule2 *aa1= crrer_initailiser_ccellule2(0,row,col);
                    int c,d,k,l;
                    for(j=1;j<=4;j++)
                    {
                        aa=add_none_first(aa,j,etw,row,col);
                        first_one_or_two(NULL,&c,&d,j,CHKS_OWNER_TWO,etw,row,col);
                        if(c==-1 && d==-1)continue;
                        else //construire l'arbre pour le champ
                        {
                            etat_window *etw1=creer_intialiser_etat_window();
                            copie_win_field1(etw,etw1);
                            aa1=queen_arbre1(aa1,j,etw1,c,d,
                                             CHKS_OWNER_TWO,CHKS_OWNER_ONE);
                        }
                    }
                    if(aa1->topright || aa1->topleft || aa1->butright || aa1->butleft)
                        aa=inserer_debut(aa,aa1);
                    cl[i]=aa;
                }
            }
            break;
        case CHKS_OWNER_TWO://cas de la machine
            taille=etw->machine->nbr_piece_disp;
            cl=(cellule2**) malloc(sizeof (cellule2*)*taille);
            if(!cl)
            {
                printf("error allocation");
                exit(-1);
            }
            for(n=0;n<taille;n++)cl[n]=NULL;
            for(i=0;i<taille;i++)
            {
                row=etw->machine->Tab[i].x;
                col=etw->machine->Tab[i].y;
                if(!etw->filed[row-1][col-1]->is_queen)
                {
                    if( (row-1-1>=0) && (col-1-1>=0))
                    {
                        if(etw->filed[row-1-1][col-1-1]->owner==CHKS_OWNER_NONE)//left
                            cl[i]=inserer_fin_liste_cellule2(cl[i],0,row-1,col-1);
                    }
                    if((row-1-1>=0) && (col+1-1<=7))
                    {
                        if(etw->filed[row-1-1][col+1-1]->owner==CHKS_OWNER_NONE)//right
                            cl[i]=inserer_fin_liste_cellule2(cl[i],0,row-1,col+1);
                    }
                    cellule2 *ne_test= crrer_initailiser_ccellule2(0,row,col);
                    ne_test=construire_arbre1(etw,ne_test,row,col);
                    if(ne_test->topleft || ne_test->topright)
                    {
                        if(!cl[i]) cl[i]=ne_test;
                        else {
                            ne_test->suiv = cl[i];
                            cl[i] = ne_test;
                        }
                    }
                }else//  if queen ,a traité
                {
                    cellule2 *aa=NULL;
                    cellule2 *aa1= crrer_initailiser_ccellule2(0,row,col);
                    int c,d,k,l;
                    for(j=1;j<=4;j++)
                    {
                        aa=add_none_first(aa,j,etw,row,col);
                        first_one_or_two(NULL,&c,&d,j,CHKS_OWNER_ONE,etw,row,col);
                        if(c==-1 && d==-1)continue;
                        else //construire l'arbre pour le champ
                        {
                            etat_window *etw1=creer_intialiser_etat_window();
                            copie_win_field1(etw,etw1);
                            //(c) et la ligne de primiere one et (d) sa colonon
                            aa1=queen_arbre1(aa1,j,etw1,c,d,CHKS_OWNER_ONE,CHKS_OWNER_TWO);
                        }
                    }
                    if(aa1->topright || aa1->topleft || aa1->butright || aa1->butleft)
                        aa=inserer_debut(aa,aa1);
                    cl[i]=aa;
                }
            }
            break;
    }
    return (cellule2**)cl;
}

// Fonction récursive pour trouver tous les chemins d'un arbre binaire
CelluleList *findPaths (cellule2 *node, int path[2][8], int pathLength, CelluleList *listes)
{
    if (node == NULL) return ((CelluleList *)listes);

    // Ajouter le noeud actuel au chemin
    path[0][pathLength] = node->row;
    path[1][pathLength] = node->col;
    pathLength++;

    // Si le noeud actuel est une feuille, imprimer le chemin
    if (node->topleft == NULL && node->topright == NULL &&
        node->butleft == NULL && node->butright == NULL)
    {
        cellule7 *newList = NULL;
        for (int i = 0; i < pathLength; i++) newList = addToEndOfListe(newList, path[0][i], path[1][i]);
        listes = addList(listes, newList);
    }

    // Sinon, explorer les sous-arbres de manière récursive
    listes = findPaths(node->topleft, path, pathLength, listes);
    listes = findPaths(node->topright, path, pathLength, listes);
    listes = findPaths(node->butleft, path, pathLength, listes);
    listes = findPaths(node->butright, path, pathLength, listes);
    if (pathLength != 1) listes = findPaths(node->suiv, path, pathLength-1, listes);
    return ((CelluleList *)listes);
}

cellule5 *generate_all_path(cellule2 **cl,int taille)
{
    cellule5 *list=NULL;
    int path[2][8];
    int pathLength = 0;
    cellule5 *ne;
    int i;
    for(i=0;i<taille;i++)
    {
        cellule2 *p;
        p=cl[i];
        while (p)
        {
            CelluleList *l1=NULL;
            l1=findPaths(p,path,pathLength,l1);
            CelluleList *p1;
            p1=l1;
            while (p1)//probleme ici
            {//
                ne= creer_intialiser_cellule5(p1->liste,i);
                list=insert_into_cellule5_liste(list,ne);
                p1=p1->suiv;
            }
            p=p->suiv;
        }

    }
    return (cellule5*)list;
}

void change_row_col_piece(etat_window *etw,ChksField *field,int lrow,int lcol,ChksOwner owner)
{
    int i;
    if(!etw)
    {
        switch (owner) {
            case CHKS_OWNER_ONE:
                for(i=0;i<field->view_table->user->nbr_piece_disp;i++)
                {
                    if(field->view_table->user->Tab[i].x==lrow &&
                       field->view_table->user->Tab[i].y==lcol)
                    {
                        field->view_table->user->Tab[i].x=field->row;
                        field->view_table->user->Tab[i].y=field->col;
                    }
                }
                break;
            case CHKS_OWNER_TWO:
                for(i=0;i<field->view_table->machine->nbr_piece_disp;i++)
                {
                    if(field->view_table->machine->Tab[i].x==lrow &&
                       field->view_table->machine->Tab[i].y==lcol)
                    {
                        field->view_table->machine->Tab[i].x=field->row;
                        field->view_table->machine->Tab[i].y=field->col;
                    }
                }
                break;
        }
    }else
    {
        switch (owner) {
            case CHKS_OWNER_ONE:
                for(i=0;i<etw->user->nbr_piece_disp;i++)
                {
                    if(etw->user->Tab[i].x==lrow &&
                       etw->user->Tab[i].y==lcol)
                    {
                        etw->user->Tab[i].x=field->row;
                        etw->user->Tab[i].y=field->col;
                    }
                }
                break;
            case CHKS_OWNER_TWO:
                for(i=0;i<etw->machine->nbr_piece_disp;i++)
                {
                    if(etw->machine->Tab[i].x==lrow &&
                       etw->machine->Tab[i].y==lcol)
                    {
                        etw->machine->Tab[i].x=field->row;
                        etw->machine->Tab[i].y=field->col;
                    }
                }
                break;
        }
    }
}

int Minmaxalgorithm(ChksOwner owner,int depth,etat_window *etw,bestpath *bs)
{
    int max,min,row,col,rowdest,coldest,i,j;
    GtkStyleContext* context;
    if(depth==0)
        return (int)(etw->machine->nbr_piece_disp-etw->user->nbr_piece_disp);
    if(owner==CHKS_OWNER_ONE) //user
    {
        cellule2 **cl=NULL;
        cl=table_arbre_shemin(cl,etw,CHKS_OWNER_ONE);
        cellule5 *ll=NULL;
        ll=generate_all_path(cl,etw->user->nbr_piece_disp);
        freeallcl(cl,etw->user->nbr_piece_disp);
        cellule5 *p;
        p=ll;
        while (p)
        {
            etat_window *etw1=creer_intialiser_etat_window();
            copie_win_field1(etw,etw1);
            //effectuer le mouvement sur etw1
            File *f=creer_initaliser_file();
            transfert_cellule7_cellule(f,p->liste);
            ChksField *last;
            ChksField *field;
            row=etw1->user->Tab[p->numfield].x;
            col=etw1->user->Tab[p->numfield].y;
            last=etw1->filed[row-1][col-1];
            rowdest=f->queue->row;
            coldest=f->queue->col;
            field = etw1->filed[rowdest - 1][coldest - 1];
            if(f->tete!=f->queue) {//plusieurs deplacement
                drop_two_betwen_fl_lt1(etw1, f, CHKS_OWNER_TWO,row,col,rowdest,coldest);
            }
            else //un seul deplacement
            {
                drop_two_betwen_fl_lt1(etw1,NULL,CHKS_OWNER_TWO,row,col,rowdest,coldest);
            }
            if(rowdest==8)
            {
                field->is_queen=TRUE;
                last->is_queen=FALSE;
            }
            last->owner=CHKS_OWNER_NONE;
            field->owner=CHKS_OWNER_ONE;
            if(last->is_queen)
            {
                field->is_queen=TRUE;
                last->is_queen=FALSE;
            }
            change_row_col_piece(etw1,field,row,col,CHKS_OWNER_ONE);
            p->score=Minmaxalgorithm(CHKS_OWNER_TWO,depth-1,etw1,bs);
            free(etw1->user);
            free(etw1->machine);
            for(i=0;i<8;i++)
                for (j=0;j<8;j++)
                    free(etw1->filed[i][j]);
            free(etw1);
            p=p->suiv;
        }
        if(ll)
        {
            min=minimun_cellule5(ll);
            freell(ll);
        }
        else return (int)2222;//rien a deplacer pour user donc c'est le bon chemin a effectuer
        return (int)min;
    }else if(owner==CHKS_OWNER_TWO)//machine
    {
        cellule2 **cl=NULL;
        //creer la table des chemein de la machine sur l'etat actuelle ede la table
        cl=table_arbre_shemin(cl,etw,CHKS_OWNER_TWO);
        cellule5 *ll=NULL;
        //generer les chemin de chaque piece est les stocker dans une liste de liste
        ll=generate_all_path(cl,etw->machine->nbr_piece_disp);
        freeallcl(cl,etw->machine->nbr_piece_disp);
        cellule5 *p;
        p=ll;
        //boucler sur les chemin pour tout les piece et effectuer le mouvement
        while (p)
        {
            etat_window *etw1=creer_intialiser_etat_window();
            copie_win_field1(etw,etw1);
            //effectuer le mouvement sur etw1
            File *f=creer_initaliser_file();
            transfert_cellule7_cellule(f,p->liste);
            ChksField *last;
            ChksField *field;
            row=etw1->machine->Tab[p->numfield].x;//source row
            col=etw1->machine->Tab[p->numfield].y;//source col
            last=etw1->filed[row-1][col-1];
            rowdest=f->queue->row;//destination row
            coldest=f->queue->col;//destination col
            field = etw1->filed[rowdest - 1][coldest - 1];
            if(f->tete!=f->queue)
            {//plusieurs deplacement
                drop_two_betwen_fl_lt1(etw1, f, CHKS_OWNER_ONE,row,col,rowdest,coldest);
            }
            else //un seule deplacement
            {
                drop_two_betwen_fl_lt1(etw1,NULL,CHKS_OWNER_ONE,row,col,rowdest,coldest);
            }
            if(rowdest==1)
            {
                field->is_queen=TRUE;
                last->is_queen=FALSE;
            }
            last->owner=CHKS_OWNER_NONE;
            field->owner=CHKS_OWNER_TWO;
            if(last->is_queen)
            {
                field->is_queen=TRUE;
                last->is_queen=FALSE;
            }
            //last est la destination
            change_row_col_piece(etw1,field,row,col,CHKS_OWNER_TWO);
            p->score=Minmaxalgorithm(CHKS_OWNER_ONE,depth-1,etw1,bs);
            p=p->suiv;
            free(etw1->user);
            free(etw1->machine);
            for(i=0;i<8;i++)
                for (j=0;j<8;j++)
                    free(etw1->filed[i][j]);
            free(etw1);
        }
        if(ll)max=maximun_cellule5(ll);
        else return (int)-2222;//la machine ne peut pas effectuer un mouvement
        //il faut stocker le bon chemin dans best
        p=ll;
        while (p)
        {
            if(p->score==max)
            {
                transfert_cellule7_cellule(bs->best,p->liste);
                bs->numfield=p->numfield;
                bs->score=p->score;
                break;
            }
            p=p->suiv;
        }
        freell(ll);
        return (int)max;
    }
}

int Minmaxalgorithm2(ChksOwner owner,int depth,etat_window *etw,bestpath *bs)
{
    int max,min,row,col,rowdest,coldest,i,j;
    GtkStyleContext* context;
    if(depth==0)
        return (int)(etw->user->nbr_piece_disp-etw->machine->nbr_piece_disp);
    if(owner==CHKS_OWNER_TWO) //machine
    {
        cellule2 **cl=NULL;
        cl=table_arbre_shemin(cl,etw,CHKS_OWNER_TWO);
        cellule5 *ll=NULL;
        ll=generate_all_path(cl,etw->machine->nbr_piece_disp);
        freeallcl(cl,etw->machine->nbr_piece_disp);
        cellule5 *p;
        p=ll;
        while (p)
        {
            etat_window *etw1=creer_intialiser_etat_window();
            copie_win_field1(etw,etw1);
            //effectuer le mouvement sur etw1
            File *f=creer_initaliser_file();
            transfert_cellule7_cellule(f,p->liste);
            ChksField *last;
            ChksField *field;
            row=etw1->machine->Tab[p->numfield].x;
            col=etw1->machine->Tab[p->numfield].y;
            last=etw1->filed[row-1][col-1];
            rowdest=f->queue->row;
            coldest=f->queue->col;
            field = etw1->filed[rowdest - 1][coldest - 1];
            if(f->tete!=f->queue) {//plusieurs deplacement
                drop_two_betwen_fl_lt1(etw1, f, CHKS_OWNER_ONE,row,col,rowdest,coldest);
            }
            else //un seul deplacement
            {
                drop_two_betwen_fl_lt1(etw1,NULL,CHKS_OWNER_ONE,row,col,rowdest,coldest);
            }
            if(rowdest==1)
            {
                field->is_queen=TRUE;
                last->is_queen=FALSE;
            }
            last->owner=CHKS_OWNER_NONE;
            field->owner=CHKS_OWNER_TWO;
            if(last->is_queen)
            {
                field->is_queen=TRUE;
                last->is_queen=FALSE;
            }
            change_row_col_piece(etw1,field,row,col,CHKS_OWNER_TWO);
            p->score=Minmaxalgorithm2(CHKS_OWNER_ONE,depth-1,etw1,bs);
            p=p->suiv;
            free(etw1->user);
            free(etw1->machine);
            for(i=0;i<8;i++)
                for (j=0;j<8;j++)
                    free(etw1->filed[i][j]);
            free(etw1);
        }
        if(ll)
        {
            min=minimun_cellule5(ll);
            freell(ll);
        }
        else return (int)2222;//rien a deplacer pour user donc c'est le bon chemin a effectuer
        return (int)min;
    }else if(owner==CHKS_OWNER_ONE)//machine
    {
        cellule2 **cl=NULL;
        //creer la table des chemein de la machine sur l'etat actuelle ede la table
        cl=table_arbre_shemin(cl,etw,CHKS_OWNER_ONE);
        cellule5 *ll=NULL;
        //generer les chemin de chaque piece est les stocker dans une liste de liste
        ll=generate_all_path(cl,etw->user->nbr_piece_disp);
        freeallcl(cl,etw->user->nbr_piece_disp);
        cellule5 *p;
        p=ll;
        //boucler sur les chemin pour tout les piece et effectuer le mouvement
        while (p)
        {
            etat_window *etw1=creer_intialiser_etat_window();
            copie_win_field1(etw,etw1);
            //effectuer le mouvement sur etw1
            File *f=creer_initaliser_file();
            transfert_cellule7_cellule(f,p->liste);
            ChksField *last;
            ChksField *field;
            row=etw1->user->Tab[p->numfield].x;//source row
            col=etw1->user->Tab[p->numfield].y;//source col
            last=etw1->filed[row-1][col-1];
            rowdest=f->queue->row;//destination row
            coldest=f->queue->col;//destination col
            field = etw1->filed[rowdest - 1][coldest - 1];
            if(f->tete!=f->queue)
            {//plusieurs deplacement
                drop_two_betwen_fl_lt1(etw1, f, CHKS_OWNER_TWO,row,col,rowdest,coldest);
            }
            else //un seule deplacement
            {
                drop_two_betwen_fl_lt1(etw1,NULL,CHKS_OWNER_TWO,row,col,rowdest,coldest);
            }
            if(rowdest==8)
            {
                field->is_queen=TRUE;
                last->is_queen=FALSE;
            }
            last->owner=CHKS_OWNER_NONE;
            field->owner=CHKS_OWNER_ONE;
            if(last->is_queen)
            {
                field->is_queen=TRUE;
                last->is_queen=FALSE;
            }
            //last est la destination
            change_row_col_piece(etw1,field,row,col,CHKS_OWNER_ONE);
            p->score=Minmaxalgorithm2(CHKS_OWNER_TWO,depth-1,etw1,bs);
            p=p->suiv;
            free(etw1->user);
            free(etw1->machine);
            for(i=0;i<8;i++)
                for (j=0;j<8;j++)
                    free(etw1->filed[i][j]);
            free(etw1);
        }
        if(ll)max=maximun_cellule5(ll);
        else return (int)-2222;//la machine ne peut pas effectuer un mouvement
        //il faut stocker le bon chemin dans best
        p=ll;
        while (p)
        {
            if(p->score==max)
            {
                transfert_cellule7_cellule(bs->best,p->liste);
                bs->numfield=p->numfield;
                bs->score=p->score;
                break;
            }
            p=p->suiv;
        }
        freell(ll);
        return (int)max;
    }
}

void arbre_descicion(Glob *g)
{
    GtkStyleContext* context;
    cellule2 **cl;
    int row,col,rowdest,coldest,et;
    ChksField *last,*field;
    etat_window *etw=creer_intialiser_etat_window();
    copie_win_field(g->field->view_table,etw);
    bestpath *p=(bestpath*) malloc(sizeof (bestpath));
    p->best=creer_initaliser_file();
    et=Minmaxalgorithm(CHKS_OWNER_TWO,g->field->view_table->depth,etw,p);
    if(!p->best->tete)
    {
        //machine lose
        g->field->view_table->machine->nbr_piece_disp=0;
        return;
   }
    row=g->field->view_table->machine->Tab[p->numfield].x;
    col=g->field->view_table->machine->Tab[p->numfield].y;
    last=g->field->view_table->table->filed[row-1][col-1];
    rowdest=p->best->queue->row;
    coldest=p->best->queue->col;
    field = g->field->view_table->table->filed[rowdest - 1][coldest - 1];
    if(p->best->tete!=p->best->queue) {//plusieurs deplacement
        drop_two_betwen_fl_lt(last, field, p->best, CHKS_OWNER_ONE);
    }
    else //un seul deplacement
    {
        drop_two_betwen_fl_lt(last,field,NULL,CHKS_OWNER_ONE);
    }
    if(rowdest==1)
    {
        field->is_queen=TRUE;
        last->is_queen=FALSE;
    }
    last->owner=CHKS_OWNER_NONE;
    field->owner=CHKS_OWNER_TWO;
    if(last->is_queen)
    {
        field->is_queen=TRUE;
        last->is_queen=FALSE;
    }
    change_row_col_piece(NULL,field,last->row,
                         last->col,CHKS_OWNER_TWO);
    apply_style_button(last->view_table->table->filed);
    //user lose
    if(p->score==2222)
    {
        g->field->view_table->user->nbr_piece_disp=0;
    }
}

int init_player(ChkPlayer *pl, ChksOwner owner)
{
  int i, j, n, m, k = 0;
  if (owner == CHKS_OWNER_ONE) n = 1;
	else if (owner == CHKS_OWNER_TWO) n = 6;
	else return ((int)0);
	m = n + 2;
  for (i = n; i <= m; i++)
  {
    for (j = (i%2)+1; j <= 8; j+=2)
    {
      pl->Tab[k].x=i;
      pl->Tab[k].y=j;
      k++;
    }
  }
	pl->nbr_piece_disp = 12;
	return ((int)1);
}


void init_table(Chkswindow *win)
{
    int i,j,k=0,m=0;
    GtkStyleContext* context;
    GtkCssProvider* provider;
    win->table->gr=gtk_grid_new();
    win->table->last=NULL;
    for (i=0;i<=7;i++)
    {
        for(j=0;j<=7;j++)
        {
            win->table->filed[i][j]=creer_field();
            win->table->filed[i][j]->view_table=NULL;
            win->table->filed[i][j]->button=gtk_button_new();
            context = gtk_widget_get_style_context(win->table->filed[i][j]->button);
            gtk_style_context_add_class(GTK_STYLE_CONTEXT(context),"chks-field");
            gtk_container_set_border_width(GTK_CONTAINER(win->table->filed[i][j]->button),7);
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
    add_filed_grid(win,win->table->filed);
}

gboolean BotVsBot(Chkswindow *win)
{
    int row, col, rowdest, coldest;
    bestpath *p;
    ChksField *last, *field;
    etat_window *etw;
    if(win->machine->nbr_piece_disp==0 || win->user->nbr_piece_disp==0)
    {
        if(win->machine->nbr_piece_disp==0)
        {
            gtk_header_bar_set_title(GTK_HEADER_BAR(win->header_bar),"Bleu Win");
        }else if(win->user->nbr_piece_disp==0)
        {
            gtk_header_bar_set_title(GTK_HEADER_BAR(win->header_bar), "Rouge Win");
        }
        return FALSE;
    }
    if(win->round==1)
    {
        win->round=2;
        etw = creer_intialiser_etat_window();
        copie_win_field(win, etw);
        p = (bestpath *) malloc(sizeof(bestpath));
        p->best = creer_initaliser_file();
        Minmaxalgorithm2(CHKS_OWNER_ONE, win->depth, etw, p);
        if (!p->best->tete) {
            //machine lose
            win->user->nbr_piece_disp = 0;
            return TRUE;
        }
        row = win->user->Tab[p->numfield].x;
        col = win->user->Tab[p->numfield].y;
        last = win->table->filed[row - 1][col - 1];
        rowdest = p->best->queue->row;
        coldest = p->best->queue->col;
        field = win->table->filed[rowdest - 1][coldest - 1];
        if (p->best->tete != p->best->queue) {//plusieurs deplacement
            drop_two_betwen_fl_lt(last, field, p->best, CHKS_OWNER_TWO);
        } else //un seul deplacement
        {
            drop_two_betwen_fl_lt(last, field, NULL, CHKS_OWNER_TWO);
        }
        if (rowdest == 8) {
            field->is_queen = TRUE;
            last->is_queen = FALSE;
        }
        last->owner = CHKS_OWNER_NONE;
        field->owner = CHKS_OWNER_ONE;
        if (last->is_queen) {
            field->is_queen = TRUE;
            last->is_queen = FALSE;
        }
        change_row_col_piece(NULL, field, last->row,
                             last->col, CHKS_OWNER_ONE);
        apply_style_button(win->table->filed);
    }else
    {
        win->round=1;
        etw = creer_intialiser_etat_window();
        copie_win_field(win, etw);
        p = (bestpath *) malloc(sizeof(bestpath));
        p->best = creer_initaliser_file();
        Minmaxalgorithm(CHKS_OWNER_TWO, win->depth, etw, p);
        if (!p->best->tete) {
            //machine lose
            win->machine->nbr_piece_disp = 0;
            return TRUE;
        }
        row = win->machine->Tab[p->numfield].x;
        col = win->machine->Tab[p->numfield].y;
        last = win->table->filed[row - 1][col - 1];
        rowdest = p->best->queue->row;
        coldest = p->best->queue->col;
        field = win->table->filed[rowdest - 1][coldest - 1];
        if (p->best->tete != p->best->queue) {//plusieurs deplacement
            drop_two_betwen_fl_lt(last, field, p->best, CHKS_OWNER_ONE);
        } else //un seul deplacement
        {
            drop_two_betwen_fl_lt(last, field, NULL, CHKS_OWNER_ONE);
        }
        if (rowdest == 1) {
            field->is_queen = TRUE;
            last->is_queen = FALSE;
        }
        last->owner = CHKS_OWNER_NONE;
        field->owner = CHKS_OWNER_TWO;
        if (last->is_queen) {
            field->is_queen = TRUE;
            last->is_queen = FALSE;
        }
        change_row_col_piece(NULL, field, last->row,
                             last->col, CHKS_OWNER_TWO);
        apply_style_button(win->table->filed);
    }

    afficher_table(win);

    return TRUE;

}

void Principale_fonction()
{
    Chkswindow *win = creer_Chkswindow();
    make_window(win);    
}

int main(int argc,char *argv[])
{
    gtk_init(&argc, &argv);
    
    Principale_fonction();

    gtk_main();
    return 0;
}