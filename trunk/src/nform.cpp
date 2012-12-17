#include "nform.h"
#include "mbstring.h"


NForm::NForm(int rows, int cols) : NGroup(NRect(rows,cols,0,0))
{
    frm = new_form(NULL);
    scale_form(frm,&rows,&cols);
    set_form_win(frm, win);
    set_form_sub(frm, derwin(win, rows, cols, 2,2));
    wattrset(win,getcolorpair(COLOR_WHITE, COLOR_BLACK) | A_BOLD);

//    framewin = win;
//    win = frm->sub;
//post_form(frm);
    title = NULL;
    needrefresh = true;
    //перемещаем в центр экрана
    this->move(getmaxy(stdscr)/2-getheight()/2,getmaxx(stdscr)/2-getwidth()/2);
    curs_set(1); //курсор
}


NForm::~NForm()
{
//    win = framewin; //востанавливаем для правильной деструкции
    unpost_form(frm);
    free_form(frm);
    if (title != NULL)
	free(title);
    curs_set(0); //курсор
}


void NForm::refresh()
{
    box(frm->win, ACS_VLINE, ACS_HLINE);
    if(title != NULL)
    {
	mvwprintw(frm->win,0,getwidth()/2-mbstrlen(title)/2,"%s",title); //посередине
    }
pos_form_cursor(frm); //восстановить позицию курсора ( после mvprintw() )
    NGroup::refresh();

}


void NForm::eventhandle(NEvent* ev) 	//обработчик событий
{
    NGroup::eventhandle(ev); //предок
    if ( ev->done )
	return;
    if ( ev->type == NEvent::evKB )
    {
	ev->done = true;
        switch(ev->keycode)
	{
	    case '\t':
		form_driver(frm, REQ_NEXT_FIELD);
		form_driver(frm, REQ_END_LINE);
		break;
	    case KEY_BACKSPACE:
		form_driver(frm, REQ_DEL_PREV);
		break;
	    case KEY_LEFT:
		form_driver(frm, REQ_PREV_CHAR);
		break;
	    case KEY_RIGHT:
		form_driver(frm, REQ_NEXT_CHAR);
		break;
	    case KEY_DC:
		form_driver(frm, REQ_DEL_CHAR);
		break;
	    default:
		form_driver(frm,ev->keycode); //передаем в форму
		//ev->done = false; //нет реакции на этот код
	} //switch
	if (ev->done) //если обработали, то нужно перерисоваться
	    refresh();
    }
}

