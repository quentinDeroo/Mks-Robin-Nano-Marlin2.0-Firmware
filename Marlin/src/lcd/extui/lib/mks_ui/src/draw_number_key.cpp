#include "../../../../../MarlinCore.h"
#if ENABLED(TFT_LITTLE_VGL_UI)
#include "lv_conf.h"
//#include "../../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../../lvgl/src/lv_objx/lv_img.h"
//#include "../../lvgl/src/lv_core/lv_disp.h"
//#include "../../lvgl/src/lv_core/lv_refr.h"
//#include "../../MarlinCore.h"
#include "../../../../../../Configuration.h"
#include "../inc/draw_ui.h"
#include "../../../../../module/temperature.h"
#include "../../../../../gcode/queue.h"
#if ENABLED(POWER_LOSS_RECOVERY)
#include "../../../../../feature/powerloss.h"
#endif

#include "../../../../../gcode/gcode.h"
#include "../../../../../module/planner.h"

#if HAS_TRINAMIC_CONFIG
#include "../../../../../module/stepper/indirection.h"
#include "../../../../../feature/tmc_util.h"
#endif

static lv_obj_t * scr;
static lv_obj_t *buttonValue=NULL;
static lv_obj_t *labelValue=NULL;

static char key_value[11]={0};
static uint8_t cnt=0;
static char point_flg = 1;

#define ID_NUM_KEY1		1
#define ID_NUM_KEY2		2
#define ID_NUM_KEY3		3
#define ID_NUM_KEY4		4
#define ID_NUM_KEY5		5
#define ID_NUM_KEY6		6
#define ID_NUM_KEY7		7
#define ID_NUM_KEY8		8
#define ID_NUM_KEY9		9
#define ID_NUM_KEY0		10
#define ID_NUM_BACK		11
#define ID_NUM_RESET		12
#define ID_NUM_CONFIRM		13
#define ID_NUM_POINT		14
#define ID_NUM_NAGETIVE	15

static void disp_key_value()
{
    char *temp;
    #if HAS_TRINAMIC_CONFIG
    float milliamps;
    #endif
	
    switch(value)
    {
       case PrintAcceleration:
        memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%.1f",planner.settings.acceleration);       
        break;   
       case RetractAcceleration:
        memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%.1f",planner.settings.retract_acceleration);       
        break;   
       case TravelAcceleration:
        memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%.1f",planner.settings.travel_acceleration);       
        break;  
       case XAcceleration:
        memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%d",(int)planner.settings.max_acceleration_mm_per_s2[X_AXIS]);       
        break; 
       case YAcceleration:
        memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%d",(int)planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);       
        break; 
       case ZAcceleration:
        memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%d",(int)planner.settings.max_acceleration_mm_per_s2[Z_AXIS]);       
        break;
       case E0Acceleration:
        memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%d",(int)planner.settings.max_acceleration_mm_per_s2[E_AXIS]);       
        break; 
       case E1Acceleration:
        memset(public_buf_m,0,sizeof(public_buf_m));
	 #if ENABLED(DISTINCT_E_FACTORS) && E_STEPPERS > 1
        sprintf(public_buf_m,"%d",(int)planner.settings.max_acceleration_mm_per_s2[XYZE]);   
	 #else
	 sprintf(public_buf_m,"%d",(int)planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
	 #endif
        break;
	 case XMaxFeedRate:
        memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%.1f",planner.settings.max_feedrate_mm_s[X_AXIS]);        
        break;
       case YMaxFeedRate:
        memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%.1f",planner.settings.max_feedrate_mm_s[Y_AXIS]);        
        break; 
       case ZMaxFeedRate:
        memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%.1f",planner.settings.max_feedrate_mm_s[Z_AXIS]);        
        break;  
       case E0MaxFeedRate:
        memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%.1f",planner.settings.max_feedrate_mm_s[E_AXIS]);        
        break;
       case E1MaxFeedRate:
        memset(public_buf_m,0,sizeof(public_buf_m));
	 #if ENABLED(DISTINCT_E_FACTORS) && E_STEPPERS > 1
        sprintf(public_buf_m,"%.1f",planner.settings.max_feedrate_mm_s[XYZE]);        
	 #else
	 sprintf(public_buf_m,"%.1f",planner.settings.max_feedrate_mm_s[E_AXIS]);
	 #endif
	break;
	#if HAS_CLASSIC_JERK
       case XJerk:
	
       memset(public_buf_m,0,sizeof(public_buf_m));
       sprintf(public_buf_m,"%.1f",planner.max_jerk[X_AXIS]);
	 
        break;
       case YJerk:
	
       memset(public_buf_m,0,sizeof(public_buf_m));
       sprintf(public_buf_m,"%.1f",planner.max_jerk[Y_AXIS]);
	
        break; 
       case ZJerk:
	
       memset(public_buf_m,0,sizeof(public_buf_m));
       sprintf(public_buf_m,"%.1f",planner.max_jerk[Z_AXIS]);
	 
        break; 
       case EJerk:
       memset(public_buf_m,0,sizeof(public_buf_m));
       sprintf(public_buf_m,"%.1f",planner.max_jerk[E_AXIS]);
	 
        break; 
	#endif
       case Xstep:
       memset(public_buf_m,0,sizeof(public_buf_m));
       sprintf(public_buf_m,"%.1f",planner.settings.axis_steps_per_mm[X_AXIS]);

        break; 
       case Ystep:
       memset(public_buf_m,0,sizeof(public_buf_m));
       sprintf(public_buf_m,"%.1f",planner.settings.axis_steps_per_mm[Y_AXIS]);

        break;  
       case Zstep:
       memset(public_buf_m,0,sizeof(public_buf_m));
       sprintf(public_buf_m,"%.1f",planner.settings.axis_steps_per_mm[Z_AXIS]);

        break; 
       case E0step:
       memset(public_buf_m,0,sizeof(public_buf_m));
       sprintf(public_buf_m,"%.1f",planner.settings.axis_steps_per_mm[E_AXIS]);

        break; 
       case E1step:
       memset(public_buf_m,0,sizeof(public_buf_m));
	#if ENABLED(DISTINCT_E_FACTORS) && E_STEPPERS > 1
       sprintf(public_buf_m,"%.1f",planner.settings.axis_steps_per_mm[XYZE]);
	#else
	sprintf(public_buf_m,"%.1f",planner.settings.axis_steps_per_mm[E_AXIS]);
	#endif
	
        break;  
	 
	 case Xcurrent:
	 #if AXIS_IS_TMC(X)
	 memset(public_buf_m,0,sizeof(public_buf_m));
	 milliamps=stepperX.getMilliamps();
	 sprintf(public_buf_m,"%.1f",milliamps);
	 #endif
	 break;
	  
	 case Ycurrent:
	 #if AXIS_IS_TMC(Y)
	 memset(public_buf_m,0,sizeof(public_buf_m));
	 milliamps=stepperY.getMilliamps();
	 sprintf(public_buf_m,"%.1f",milliamps);
	 #endif
	 break;
	 	 
	 case Zcurrent:
	 #if AXIS_IS_TMC(Z)
	 memset(public_buf_m,0,sizeof(public_buf_m));
	 milliamps=stepperZ.getMilliamps();
	 sprintf(public_buf_m,"%.1f",milliamps);
	 #endif
	 break;	 
	 
	 case E0current:
	 #if AXIS_IS_TMC(E0)
	 memset(public_buf_m,0,sizeof(public_buf_m));
	 milliamps=stepperE0.getMilliamps();
	 sprintf(public_buf_m,"%.1f",milliamps);
	 #endif
	 break;
	 
	 case E1current:
	 #if AXIS_IS_TMC(E1)
	 memset(public_buf_m,0,sizeof(public_buf_m));
	 milliamps=stepperE1.getMilliamps();
	 sprintf(public_buf_m,"%.1f",milliamps);
	 #endif
	 break;
	 
	 case pause_pos_x:
	 memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%.1f",gCfgItems.pausePosX);
	 break;
	 case pause_pos_y:
	 memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%.1f",gCfgItems.pausePosY);
	 break;
	 case pause_pos_z:
	 memset(public_buf_m,0,sizeof(public_buf_m));
        sprintf(public_buf_m,"%.1f",gCfgItems.pausePosZ);
	 break;
    }
    memset(key_value,0,sizeof(key_value));
    strcpy(key_value,public_buf_m);
    cnt=strlen(key_value);
    temp=strchr(key_value,'.');
    if(temp)
    {
       point_flg = 0; 
    }
    else
    {
        point_flg = 1;
    }    
    lv_label_set_text(labelValue, key_value);
    lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
    
}

static void set_value_confirm()
{
    #if HAS_TRINAMIC_CONFIG
    uint16_t current_mA;
    #endif
    switch(value)
    {   
       case PrintAcceleration:
        planner.settings.acceleration=atof(key_value);
		
        break;   
       case RetractAcceleration:
        planner.settings.retract_acceleration=atof(key_value);
                  
        break;   
       case TravelAcceleration:
        planner.settings.travel_acceleration=atof(key_value);
                        
        break;  
       case XAcceleration:
        planner.settings.max_acceleration_mm_per_s2[X_AXIS]=atof(key_value);
                  
        break; 
       case YAcceleration:
        planner.settings.max_acceleration_mm_per_s2[Y_AXIS]=atof(key_value);
                       
        break; 
       case ZAcceleration:
        planner.settings.max_acceleration_mm_per_s2[Z_AXIS]=atof(key_value);
                    
     
        break;
       case E0Acceleration:
        planner.settings.max_acceleration_mm_per_s2[E_AXIS]=atof(key_value);
                  
      
        break; 
       case E1Acceleration:
	 #if ENABLED(DISTINCT_E_FACTORS) && E_STEPPERS > 1
        planner.settings.max_acceleration_mm_per_s2[XYZE]=atof(key_value);
        #else
	 planner.settings.max_acceleration_mm_per_s2[E_AXIS]=atof(key_value);
	 #endif
        break; 
	 case XMaxFeedRate:
        planner.settings.max_feedrate_mm_s[X_AXIS]=atof(key_value);
        break;
       case YMaxFeedRate:
        planner.settings.max_feedrate_mm_s[Y_AXIS]=atof(key_value);        
        break; 
       case ZMaxFeedRate:
        planner.settings.max_feedrate_mm_s[Z_AXIS]=atof(key_value);  
        break;  
       case E0MaxFeedRate:
        planner.settings.max_feedrate_mm_s[E_AXIS]=atof(key_value);   
        break;
       case E1MaxFeedRate:
	  #if ENABLED(DISTINCT_E_FACTORS) && E_STEPPERS > 1
        planner.settings.max_feedrate_mm_s[XYZE]=atof(key_value);
        #else
	 planner.settings.max_feedrate_mm_s[E_AXIS]=atof(key_value);
	 #endif
        break; 
	#if HAS_CLASSIC_JERK
       case XJerk:	 
        planner.max_jerk[X_AXIS]=atof(key_value);
 
        break; 
       case YJerk:	
        planner.max_jerk[Y_AXIS]=atof(key_value);
          
        break; 
       case ZJerk:	
        planner.max_jerk[Z_AXIS]=atof(key_value);
             
        break; 
       case EJerk:
        planner.max_jerk[E_AXIS]=atof(key_value);
              
        break; 
	#endif
       case Xstep:
        planner.settings.axis_steps_per_mm[X_AXIS]=atof(key_value);
                   
        break;
       case Ystep:
        planner.settings.axis_steps_per_mm[Y_AXIS]=atof(key_value);
                    
        break;  
       case Zstep:
        planner.settings.axis_steps_per_mm[Z_AXIS]=atof(key_value);
                    
        break;  
       case E0step:
        planner.settings.axis_steps_per_mm[E_AXIS]=atof(key_value);
                
        break;
       case E1step:
	 #if ENABLED(DISTINCT_E_FACTORS) && E_STEPPERS > 1
        planner.settings.axis_steps_per_mm[XYZE]=atof(key_value);
	 #else
	 planner.settings.axis_steps_per_mm[E_AXIS]=atof(key_value);
	 #endif
 
	 case Xcurrent:
	 #if AXIS_IS_TMC(X)
	 current_mA=atoi(key_value);
	 stepperX.rms_current(current_mA);
	 #endif
	 break;
	 
	 case Ycurrent:
	 #if AXIS_IS_TMC(Y)
	 current_mA=atoi(key_value);
	 stepperY.rms_current(current_mA);
	 #endif
	 break;
	 
	 case Zcurrent:
	 #if AXIS_IS_TMC(Z)
	 current_mA=atoi(key_value);
	 stepperZ.rms_current(current_mA);
	  #endif
	 break;
	 
	 case E0current:
	 #if AXIS_IS_TMC(E0)
	 current_mA=atoi(key_value);
	 stepperE0.rms_current(current_mA);
	 #endif
	 break;
	 
	 case E1current:
	 #if AXIS_IS_TMC(E1)
	 current_mA=atoi(key_value);
	 stepperE1.rms_current(current_mA);
	 #endif
	 break;
	             
        break;  
	  case pause_pos_x:
	  	gCfgItems.pausePosX=atof(key_value);
		update_spi_flash();
	  	break;
	  case pause_pos_y:
	  	gCfgItems.pausePosY=atof(key_value);
		update_spi_flash();
	  	break;
	 case pause_pos_z:
	  	gCfgItems.pausePosZ=atof(key_value);
		update_spi_flash();
	  	break;
    }
   gcode.process_subcommands_now_P(PSTR("M500"));
}

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
	switch(obj->mks_obj_id)
	{
	    case ID_NUM_KEY1:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(cnt<=10)
                    {         
                        key_value[cnt]='1';
                        lv_label_set_text(labelValue, key_value);
    			    lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
                        cnt++;
                    }
	    }
	    break;
	    case ID_NUM_KEY2:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(cnt<=10)
                    {         
                        key_value[cnt]=(char)'2';
                        lv_label_set_text(labelValue, key_value);
    			    lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
                        cnt++;
                    }
	    }
	    break;
	    case ID_NUM_KEY3:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(cnt<=10)
                    {         
                        key_value[cnt]=(char)'3';
                        lv_label_set_text(labelValue, key_value);
    			    lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
                        cnt++;
                    }
	    }
	    break;
	    case ID_NUM_KEY4:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(cnt<=10)
                    {         
                        key_value[cnt]=(char)'4';
                        lv_label_set_text(labelValue, key_value);
    			    lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
                        cnt++;
                    }
	    }
	    break;
	    case ID_NUM_KEY5:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(cnt<=10)
                    {         
                        key_value[cnt]=(char)'5';
                        lv_label_set_text(labelValue, key_value);
    			    lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
                        cnt++;
                    }
	    }
	    break;
	    case ID_NUM_KEY6:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(cnt<=10)
                    {         
                        key_value[cnt]=(char)'6';
                        lv_label_set_text(labelValue, key_value);
    			    lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
                        cnt++;
                    }
	    }
	    break;
	    case ID_NUM_KEY7:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(cnt<=10)
                    {         
                        key_value[cnt]=(char)'7';
                        lv_label_set_text(labelValue, key_value);
    			    lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
                        cnt++;
                    }
	    }
	    break;
	    case ID_NUM_KEY8:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(cnt<=10)
                    {         
                        key_value[cnt]=(char)'8';
                        lv_label_set_text(labelValue, key_value);
    			    lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
                        cnt++;
                    }
	    }
	    break;
	    case ID_NUM_KEY9:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(cnt<=10)
                    {         
                        key_value[cnt]=(char)'9';
                        lv_label_set_text(labelValue, key_value);
    			    lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
                        cnt++;
                    }
	    }
	    break;
	    case ID_NUM_KEY0:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(cnt<=10)
                    {         
                        key_value[cnt]=(char)'0';
                        lv_label_set_text(labelValue, key_value);
    			    lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
                        cnt++;
                    }
	    }
	    break;
	    case ID_NUM_BACK:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(cnt>0)
                    {   
                       cnt--;  
                    }
                    if(key_value[cnt]=='.')point_flg=1;
                    key_value[cnt]=0;
                    lv_label_set_text(labelValue, key_value);
    			lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
	    }
	    break;
	    case ID_NUM_RESET:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			memset(key_value,0,sizeof(key_value));
                    cnt=0;
                    key_value[cnt]=(char)'0';
                    point_flg = 1;
                    lv_label_set_text(labelValue, key_value);
    			lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
	    }
	    break;
	    case ID_NUM_POINT:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if((cnt!=0)&&(point_flg==1))
                    {  
                        point_flg = 0;
                        key_value[cnt]=(char)'.';
                        lv_label_set_text(labelValue, key_value);
    			    lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
                        cnt++;
                    }
	    }
	    break;
	    case ID_NUM_NAGETIVE:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			if(cnt==0)
                    {         
                        key_value[cnt]=(char)'-';
                        lv_label_set_text(labelValue, key_value);
    			    lv_obj_align(labelValue, buttonValue, LV_ALIGN_CENTER,0, 0);
                        cnt++;
                    }
	    }
	    break;
	    case ID_NUM_CONFIRM:
	    if(event == LV_EVENT_CLICKED) {
			
	    }
	    else if(event == LV_EVENT_RELEASED) {
			last_disp_state = NUMBER_KEY_UI;
                    if(strlen(key_value)!=0)
                        set_value_confirm();
                    lv_clear_number_key();
                    draw_return_ui();
	    }
	    break;
	}
}

void lv_draw_number_key(void)
{
	lv_obj_t *NumberKey_1=NULL,*NumberKey_2=NULL,*NumberKey_3=NULL,*NumberKey_4=NULL,*NumberKey_5=NULL;
	lv_obj_t *NumberKey_6=NULL,*NumberKey_7=NULL,*NumberKey_8=NULL,*NumberKey_9=NULL,*NumberKey_0=NULL;
	lv_obj_t *KeyPoint=NULL,*KeyConfirm=NULL,*KeyReset=NULL,*KeyBack=NULL;
	lv_obj_t *Nagetive=NULL;
	lv_obj_t *labelKey_1=NULL,*labelKey_2=NULL,*labelKey_3=NULL,*labelKey_4=NULL,*labelKey_5=NULL;
	lv_obj_t *labelKey_6=NULL,*labelKey_7=NULL,*labelKey_8=NULL,*labelKey_9=NULL,*labelKey_0=NULL;
	lv_obj_t *labelKeyPoint=NULL,*labelKeyConfirm=NULL,*labelKeyReset=NULL,*labelKeyBack=NULL;
	lv_obj_t *labelNagetive=NULL;
	
	buttonValue=NULL;
	labelValue=NULL;
	
	if(disp_state_stack._disp_state[disp_state_stack._disp_index] != NUMBER_KEY_UI)
	{
		disp_state_stack._disp_index++;
		disp_state_stack._disp_state[disp_state_stack._disp_index] = NUMBER_KEY_UI;
	}
	disp_state = NUMBER_KEY_UI;

	scr = lv_obj_create(NULL, NULL);
	
	lv_obj_set_style(scr, &tft_style_scr);
  	lv_scr_load(scr);
  	lv_obj_clean(scr);

  	//lv_obj_t * title = lv_label_create(scr, NULL);
	//lv_obj_set_style(title, &tft_style_lable_rel);
	//lv_obj_set_pos(title,TITLE_XPOS,TITLE_YPOS);
	//lv_label_set_text(title, creat_title_text());
  
  	lv_refr_now(lv_refr_get_disp_refreshing());
	
	//LV_IMG_DECLARE(bmp_pic);
	
	buttonValue = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(buttonValue, 92,40);                            /*Set its position*/
	lv_obj_set_size(buttonValue, 296,40); 
	lv_obj_set_event_cb_mks(buttonValue, event_handler,ID_NUM_KEY1,NULL,0);
	lv_btn_set_style(buttonValue, LV_BTN_STYLE_REL, &style_num_text);    /*Set the button's released style*/
	lv_btn_set_style(buttonValue, LV_BTN_STYLE_PR, &style_num_text);      /*Set the button's pressed style*/
	lv_btn_set_layout(buttonValue, LV_LAYOUT_OFF);
	labelValue = lv_label_create(buttonValue, NULL);          /*Add a label to the button*/
	
	
	NumberKey_1 = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(NumberKey_1, 92,90);                            /*Set its position*/
	lv_obj_set_size(NumberKey_1, 68,40); 
	lv_obj_set_event_cb_mks(NumberKey_1, event_handler,ID_NUM_KEY1,NULL,0);
	lv_btn_set_style(NumberKey_1, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(NumberKey_1, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(NumberKey_1, LV_LAYOUT_OFF);
	labelKey_1 = lv_label_create(NumberKey_1, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKey_1, machine_menu.key_1);
	lv_obj_align(labelKey_1, NumberKey_1, LV_ALIGN_CENTER,0, 0);

	NumberKey_2 = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(NumberKey_2, 168,90);                            /*Set its position*/
	lv_obj_set_size(NumberKey_2, 68,40); 
	lv_obj_set_event_cb_mks(NumberKey_2, event_handler,ID_NUM_KEY2,NULL,0);
	lv_btn_set_style(NumberKey_2, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(NumberKey_2, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(NumberKey_2, LV_LAYOUT_OFF);
	labelKey_2 = lv_label_create(NumberKey_2, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKey_2, machine_menu.key_2);
	lv_obj_align(labelKey_2, NumberKey_2, LV_ALIGN_CENTER,0, 0);

	NumberKey_3 = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(NumberKey_3, 244,90);                            /*Set its position*/
	lv_obj_set_size(NumberKey_3, 68,40); 
	lv_obj_set_event_cb_mks(NumberKey_3, event_handler,ID_NUM_KEY3,NULL,0);
	lv_btn_set_style(NumberKey_3, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(NumberKey_3, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(NumberKey_3, LV_LAYOUT_OFF);
	labelKey_3 = lv_label_create(NumberKey_3, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKey_3, machine_menu.key_3);
	lv_obj_align(labelKey_3, NumberKey_3, LV_ALIGN_CENTER,0, 0);

	NumberKey_4 = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(NumberKey_4, 92,140);                            /*Set its position*/
	lv_obj_set_size(NumberKey_4, 68,40); 
	lv_obj_set_event_cb_mks(NumberKey_4, event_handler,ID_NUM_KEY4,NULL,0);
	lv_btn_set_style(NumberKey_4, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(NumberKey_4, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(NumberKey_4, LV_LAYOUT_OFF);
	labelKey_4 = lv_label_create(NumberKey_4, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKey_4, machine_menu.key_4);
	lv_obj_align(labelKey_4, NumberKey_4, LV_ALIGN_CENTER,0, 0);

	NumberKey_5 = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(NumberKey_5, 168,140);                            /*Set its position*/
	lv_obj_set_size(NumberKey_5, 68,40); 
	lv_obj_set_event_cb_mks(NumberKey_5, event_handler,ID_NUM_KEY5,NULL,0);
	lv_btn_set_style(NumberKey_5, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(NumberKey_5, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(NumberKey_5, LV_LAYOUT_OFF);
	labelKey_5 = lv_label_create(NumberKey_5, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKey_5, machine_menu.key_5);
	lv_obj_align(labelKey_5, NumberKey_5, LV_ALIGN_CENTER,0, 0);

	NumberKey_6 = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(NumberKey_6, 244,140);                            /*Set its position*/
	lv_obj_set_size(NumberKey_6, 68,40); 
	lv_obj_set_event_cb_mks(NumberKey_6, event_handler,ID_NUM_KEY6,NULL,0);
	lv_btn_set_style(NumberKey_6, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(NumberKey_6, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(NumberKey_6, LV_LAYOUT_OFF);
	labelKey_6 = lv_label_create(NumberKey_6, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKey_6, machine_menu.key_6);
	lv_obj_align(labelKey_6, NumberKey_6, LV_ALIGN_CENTER,0, 0);

	NumberKey_7 = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(NumberKey_7, 92,190);                            /*Set its position*/
	lv_obj_set_size(NumberKey_7, 68,40); 
	lv_obj_set_event_cb_mks(NumberKey_7, event_handler,ID_NUM_KEY7,NULL,0);
	lv_btn_set_style(NumberKey_7, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(NumberKey_7, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(NumberKey_7, LV_LAYOUT_OFF);
	labelKey_7 = lv_label_create(NumberKey_7, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKey_7, machine_menu.key_7);
	lv_obj_align(labelKey_7, NumberKey_7, LV_ALIGN_CENTER,0, 0);

	NumberKey_8 = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(NumberKey_8, 168,190);                            /*Set its position*/
	lv_obj_set_size(NumberKey_8, 68,40); 
	lv_obj_set_event_cb_mks(NumberKey_8, event_handler,ID_NUM_KEY8,NULL,0);
	lv_btn_set_style(NumberKey_8, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(NumberKey_8, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(NumberKey_8, LV_LAYOUT_OFF);
	labelKey_8 = lv_label_create(NumberKey_8, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKey_8, machine_menu.key_8);
	lv_obj_align(labelKey_8, NumberKey_8, LV_ALIGN_CENTER,0, 0);

	NumberKey_9 = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(NumberKey_9, 244,190);                            /*Set its position*/
	lv_obj_set_size(NumberKey_9, 68,40); 
	lv_obj_set_event_cb_mks(NumberKey_9, event_handler,ID_NUM_KEY9,NULL,0);
	lv_btn_set_style(NumberKey_9, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(NumberKey_9, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(NumberKey_9, LV_LAYOUT_OFF);
	labelKey_9 = lv_label_create(NumberKey_9, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKey_9, machine_menu.key_9);
	lv_obj_align(labelKey_9, NumberKey_9, LV_ALIGN_CENTER,0, 0);

	NumberKey_0 = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(NumberKey_0, 92,240);                            /*Set its position*/
	lv_obj_set_size(NumberKey_0, 68,40); 
	lv_obj_set_event_cb_mks(NumberKey_0, event_handler,ID_NUM_KEY0,NULL,0);
	lv_btn_set_style(NumberKey_0, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(NumberKey_0, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(NumberKey_0, LV_LAYOUT_OFF);
	labelKey_0 = lv_label_create(NumberKey_0, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKey_0, machine_menu.key_0);
	lv_obj_align(labelKey_0, NumberKey_0, LV_ALIGN_CENTER,0, 0);

	KeyBack = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(KeyBack, 320,90);                            /*Set its position*/
	lv_obj_set_size(KeyBack, 68,40); 
	lv_obj_set_event_cb_mks(KeyBack, event_handler,ID_NUM_BACK,NULL,0);
	lv_btn_set_style(KeyBack, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(KeyBack, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(KeyBack, LV_LAYOUT_OFF);
	labelKeyBack = lv_label_create(KeyBack, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKeyBack, machine_menu.key_back);
	lv_obj_align(labelKeyBack, KeyBack, LV_ALIGN_CENTER,0, 0);

	KeyReset = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(KeyReset, 320,140);                            /*Set its position*/
	lv_obj_set_size(KeyReset, 68,40); 
	lv_obj_set_event_cb_mks(KeyReset, event_handler,ID_NUM_RESET,NULL,0);
	lv_btn_set_style(KeyReset, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(KeyReset, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(KeyReset, LV_LAYOUT_OFF);
	labelKeyReset = lv_label_create(KeyReset, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKeyReset, machine_menu.key_reset);
	lv_obj_align(labelKeyReset, KeyReset, LV_ALIGN_CENTER,0, 0);

	KeyConfirm = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(KeyConfirm, 320,190);                            /*Set its position*/
	lv_obj_set_size(KeyConfirm, 68,90); 
	lv_obj_set_event_cb_mks(KeyConfirm, event_handler,ID_NUM_CONFIRM,NULL,0);
	lv_btn_set_style(KeyConfirm, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(KeyConfirm, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(KeyConfirm, LV_LAYOUT_OFF);
	labelKeyConfirm = lv_label_create(KeyConfirm, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKeyConfirm, machine_menu.key_confirm);
	lv_obj_align(labelKeyConfirm, KeyConfirm, LV_ALIGN_CENTER,0, 0);

	KeyPoint = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(KeyPoint, 244,240);                            /*Set its position*/
	lv_obj_set_size(KeyPoint, 68,40); 
	lv_obj_set_event_cb_mks(KeyPoint, event_handler,ID_NUM_POINT,NULL,0);
	lv_btn_set_style(KeyPoint, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(KeyPoint, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(KeyPoint, LV_LAYOUT_OFF);
	labelKeyPoint = lv_label_create(KeyPoint, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelKeyPoint, machine_menu.key_point);
	lv_obj_align(labelKeyPoint, KeyPoint, LV_ALIGN_CENTER,0, 0);

	Nagetive = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
	lv_obj_set_pos(Nagetive, 168,240);                            /*Set its position*/
	lv_obj_set_size(Nagetive, 68,40); 
	lv_obj_set_event_cb_mks(Nagetive, event_handler,ID_NUM_NAGETIVE,NULL,0);
	lv_btn_set_style(Nagetive, LV_BTN_STYLE_REL, &style_num_key_pre);    /*Set the button's released style*/
	lv_btn_set_style(Nagetive, LV_BTN_STYLE_PR, &style_num_key_rel);      /*Set the button's pressed style*/
	lv_btn_set_layout(Nagetive, LV_LAYOUT_OFF);
	labelNagetive = lv_label_create(Nagetive, NULL);          /*Add a label to the button*/
	lv_label_set_text(labelNagetive, machine_menu.negative);
	lv_obj_align(labelNagetive, Nagetive, LV_ALIGN_CENTER,0, 0);

	disp_key_value();

}

void lv_clear_number_key()
{
	lv_obj_del(scr);
}

#endif