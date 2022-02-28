//done

#ifndef LV_CUBIC_GUI_H
#define LV_CUBIC_GUI_H



#ifdef __cplusplus
extern "C" {
#endif



#include "lvgl.h"
#define ANIEND                      \
    while (lv_anim_count_running()) \
        lv_task_handler(); //等待动画完成
	//extern lv_img_dsc_t screen_buffer;


	extern lv_obj_t* scr1;
    extern lv_obj_t* gif_bg1;

	void lv_holo_cubic_gui(void);
	const lv_img_dsc_t* getN(int i);
	void showBoard(int* map);
	void showAnim(int* animMap, int direction, int newborn, int* map);
	void born(int i);
	void game_2048_gui_del(void);
	void load_png(void);
	void clear();
	void load_png1();

#ifdef __cplusplus
} /* extern "C" */
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#include "lvgl.h"
    extern const lv_img_dsc_t app_loading;
	extern const lv_img_dsc_t number_game;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
