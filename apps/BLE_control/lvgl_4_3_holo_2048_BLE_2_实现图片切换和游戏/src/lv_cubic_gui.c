//done

/*********************
 *      INCLUDES
 *********************/
#include "lv_cubic_gui.h"
#include "lvgl.h"
#include "images.h"
#include "number_game.h"
#include "tit.h"
#include "p1fnr9p2sj1nqc1id51nbv1jug1v374_44.h"
#include "lv_port_indev.h"
#include "common.h"
#define SIZE 4

lv_obj_t* scr1;

lv_obj_t* gif_bg1;
lv_obj_t* img[SIZE*SIZE];

void load_png()
{
    static lv_style_t default_style;
    lv_style_init(&default_style);
    lv_style_set_bg_color(&default_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_color(&default_style, LV_STATE_PRESSED, LV_COLOR_GRAY);
    lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED, LV_COLOR_BLACK);
    lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xaff));

LV_IMG_DECLARE(jiao);
lv_obj_t * gif_bg1 = lv_img_create(lv_scr_act(),NULL);// delete the app_window
lv_img_set_src(gif_bg1,&jiao);
lv_obj_align(gif_bg1, NULL, LV_ALIGN_CENTER, 0, 0); 
//此处是界面更换动画，即更换成新的界面，此处scr1就是新需要更换的界面
//当选择scr1(它不是新的对象)的时候，老的图片会被更换，但是新的没有办法加载
//lv_scr_load_anim(scr1, LV_SCR_LOAD_ANIM_MOVE_LEFT, 500, 2500, true);
}
//用于宽高同时增加的lv_anim_exec_xcb_t动画参数
static void anim_size_cb(void* var, int32_t v)
{
    lv_obj_set_size(var, v, v);
}
//用于斜向移动的lv_anim_exec_xcb_t动画参数
static void anim_pos_cb(void* var, int32_t v)
{
    lv_obj_set_pos(var, v, v);
}

/*
born a new value
the location of the animation
*/
void born(int i) {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)anim_size_cb);
    lv_anim_set_var(&a, img[i]);
    lv_anim_set_time(&a, 300);

    lv_anim_path_t path;
    lv_anim_path_init(&path);
    //lv_anim_path_set_cb(&path, lv_anim_path_ease_in);
    
    lv_anim_path_set_cb(&path, lv_anim_path_linear);

    /* path configurtaiton */
    lv_anim_set_path(&a, &path);

    lv_anim_set_values(&a, 0, 50);
    lv_anim_start(&a);

    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_values(&a, lv_obj_get_x(img[i])+25, lv_obj_get_x(img[i]));
    lv_anim_start(&a);

    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_values(&a, lv_obj_get_y(img[i])+25, lv_obj_get_y(img[i]));
    lv_anim_start(&a);

    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_img_set_offset_x);
    lv_anim_set_values(&a, -25, 0);
    lv_anim_start(&a);

    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_img_set_offset_y);
    lv_anim_set_values(&a, -25, 0);
    lv_anim_start(&a);
}

/*
* 合并动画
* i：合并的位置
*/
void zoom(int i) {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)anim_size_cb);
    lv_anim_set_var(&a, img[i]);
    lv_anim_set_delay(&a, 400);
    lv_anim_set_time(&a, 100);
	//播完后回放
    lv_anim_set_playback_delay(&a, 0);
    lv_anim_set_playback_time(&a, 100);

	//线性动画
    lv_anim_path_t path;
    lv_anim_path_init(&path);
    lv_anim_path_set_cb(&path, lv_anim_path_ease_in);
    //lv_anim_path_set_cb(&path, lv_anim_path_linear);
    lv_anim_set_path(&a, &path);

    lv_anim_set_values(&a, 50, 56);
    lv_anim_start(&a);

    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_x);
    lv_anim_set_values(&a, lv_obj_get_x(img[i]), lv_obj_get_x(img[i]) - 3);
    lv_anim_start(&a);

    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y);
    lv_anim_set_values(&a, lv_obj_get_y(img[i]), lv_obj_get_y(img[i]) - 3);
    lv_anim_start(&a);

    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_img_set_offset_x);
    lv_anim_set_values(&a, 0, 3);
    lv_anim_start(&a);

    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_img_set_offset_y);
    lv_anim_set_values(&a, 0, 3);
    lv_anim_start(&a);
}

/*
* 移动动画
* i：移动的目标对象
* direction：移动的方向，lv_obj_set_x或lv_obj_set_y
* dist：移动的距离，如1、-1
*/
void move(int i, lv_anim_exec_xcb_t direction, int dist) {
    lv_anim_t a;
    lv_anim_init(&a);

    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)direction);
    lv_anim_set_var(&a, img[i]);
    lv_anim_set_time(&a, 500);
    if (direction == (lv_anim_exec_xcb_t)lv_obj_set_x) {
        lv_anim_set_values(&a, lv_obj_get_x(img[i]), lv_obj_get_x(img[i]) + dist*58);
    }
    else {
        lv_anim_set_values(&a, lv_obj_get_y(img[i]), lv_obj_get_y(img[i]) + dist*58);
    }


    lv_anim_path_t path;
    lv_anim_path_init(&path);
    //渐进效果
    lv_anim_path_set_cb(&path, lv_anim_path_ease_in);
    /* 在动画中设置路径 */
    lv_anim_set_path(&a, &path);

    lv_anim_start(&a);

}


void lv_holo_cubic_gui(void)
{
    static lv_style_t default_style;
    lv_style_init(&default_style);
    lv_style_set_bg_color(&default_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_bg_color(&default_style, LV_STATE_PRESSED, LV_COLOR_GRAY);
    lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED, LV_COLOR_BLACK);
    lv_style_set_bg_color(&default_style, LV_STATE_FOCUSED | LV_STATE_PRESSED, lv_color_hex(0xaff));

    lv_obj_add_style(lv_scr_act(), LV_BTN_PART_MAIN, &default_style);
    lv_obj_t *act_obj = lv_scr_act(); // 获取当前活动页
    if (act_obj == scr1)
        return;
    lv_obj_clean(act_obj); // 清空此前页面


    //创建屏幕对象
   // scr1 = lv_scr_act();
    scr1 = lv_obj_create(NULL, NULL);
    lv_obj_add_style(scr1, LV_BTN_PART_MAIN, &default_style);

    for (int i = 0; i < SIZE * SIZE; i++) {
        img[i] = lv_img_create(scr1, NULL);
        lv_img_set_src(img[i], &N0);
        lv_obj_align(img[i], NULL, LV_ALIGN_IN_TOP_LEFT, 8 + i % 4 * 58, 8 + i / 4 * 58);
    }
  lv_scr_load(scr1);
    
}

void game_2048_gui_del(void)
{
    if (NULL != scr1)
    {
        lv_obj_clean(scr1);
        scr1 = NULL;
    }
}

//获取图片内容对象
const lv_img_dsc_t* getN(int i){
	switch(i){
		case 0:
			return &N0;
		case 2:
			return &N2;
		case 4:
			return &N4;
		case 8:
			return &N8;
		case 16:
			return &N16;
		case 32:
			return &N32;
		case 64:
			return &N64;
		case 128:
			return &N128;
		case 256:
			return &N256;
		case 512:
			return &N512;
		case 1024:
			return &N1024;
		case 2048:
			return &N2048;
		default:
			return &N0;
	}
}


//刷新棋盘
void showBoard(int* map){
	for(int i = 0; i<SIZE*SIZE ;i++){
		lv_img_set_src(img[i], getN(map[i]));
		lv_obj_align(img[i], NULL, LV_ALIGN_IN_TOP_LEFT, 8+i%4*58, 8+i/4*58);
    }
}
/*
* showAnim————用动画来更新棋盘
* animMap：动画的轨迹数组
* direction：移动的方向，1.上 2.下 3.左 4.右
* newborn：新棋子的位置
* map：新棋盘的地址
*/
void showAnim(int* animMap, int direction, int newborn, int* map){
    lv_anim_exec_xcb_t Normal;
    switch(direction){
    case 1:
    case 2:
        Normal = (lv_anim_exec_xcb_t)lv_obj_set_y;
        break;
    case 3:
    case 4:
        Normal = (lv_anim_exec_xcb_t)lv_obj_set_x;
        break;
    }


    //移动和合并
    for(int i =0;i<16;i++){
        if(animMap[i] > 4){
            zoom(i);
            move(i,Normal, animMap[i]-8);
        }else if(animMap[i]!=0){
            move(i,Normal, animMap[i]);
        }
    }

    //展示新棋盘
    delay(700);
    showBoard(map);

    //出现
    born(newborn);
}

void lv_holo_cubic_gui_del(void)
{
    if (NULL != scr1)
    {
        lv_obj_clean(scr1);
        scr1 = NULL;
    }
}

/* void load_png1()
{
LV_IMG_DECLARE(p1fnr9p2sj1nqc1id51nbv1jug1v374_44);
lv_obj_t * gif_bg2 = lv_img_create(lv_scr_act(),NULL);// delete the app_window
lv_img_set_src(gif_bg2,&p1fnr9p2sj1nqc1id51nbv1jug1v374_44);
lv_obj_align(gif_bg2, NULL, LV_ALIGN_CENTER, 0, 0); 
lv_scr_load_anim(gif_bg1, LV_SCR_LOAD_ANIM_NONE, 500, 2500, true);

} */


/* 
void clear()
{
 lv_img_cache_invalidate_src(&jiao);
}
 */