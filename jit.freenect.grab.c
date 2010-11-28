/*
 Copyright 2010, Jean-Marc Pelletier, Nenad Popov and Andrew Roth 
 jmp@jmpelletier.com
 
 This file is part of jit.freenect.grab.
  
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 
 */

#include <pthread.h>
#include "jit.common.h"
#include <libusb.h>
#include "libfreenect.h"
#include <time.h>

#define DEPTH_WIDTH 640
#define DEPTH_HEIGHT 480
#define RGB_WIDTH 640
#define RGB_HEIGHT 480
#define MAX_DEVICES 8

<<<<<<< HEAD
#define set_float4(a,b) {int vector_iterator;for(vector_iterator=0;vector_iterator<4;vector_iterator++)a[vector_iterator]= b;}
#define set_char16(a,b) {int vector_iterator;for(vector_iterator=0;vector_iterator<16;vector_iterator++)a[vector_iterator]= b;}

#define copy_float4(a,b) {int vector_iterator;for(vector_iterator=0;vector_iterator<4;vector_iterator++)b[vector_iterator] = a[vector_iterator];}
#define copy_char16(a,b) {int vector_iterator;for(vector_iterator=0;vector_iterator<16;vector_iterator++)b[vector_iterator] = a[vector_iterator];}

#define mult_scalar_float4(a,b,c) {int vector_iterator;for(vector_iterator=0;vector_iterator<4;vector_iterator++)c[vector_iterator]= a[vector_iterator] * b;}

typedef struct _jit_freenect_grab
{
	t_object		ob;
	char unique;
	char mode;
	freenect_device *device;
	uint32_t timestamp;
	long tilt;
	long raw_accel[3];
	double mks_accel[3];
	
} t_jit_freenect_grab;

typedef struct _grab_data
{
	freenect_pixel *rgb_data;
	freenect_depth *depth_data;
	freenect_device *device;
	uint32_t index;
	uint32_t timestamp;
} t_grab_data;
=======
typedef union _lookup_data{
	long *l_ptr;
	float *f_ptr;
	double *d_ptr;
}t_lookup;
>>>>>>> 9ac7675ae16b954ce114f9c144d6bb3806960962

enum thread_mess_type{
	NONE,
	OPEN,
	CLOSE,
	TERMINATE
};

typedef struct _jit_freenect_grab
{
	t_object         ob;
	char             unique;
	char             mode;
	char             has_frames;
	long             index;
	long             ndevices;
	freenect_device  *device;
	uint32_t         timestamp;
	t_lookup         lut;
	t_symbol         *lut_type;
	double             tilt;
	long             accelcount;
	double           mks_accel[3];
	freenect_pixel   *rgb_data;
	freenect_depth   *depth_data;
	uint32_t         rgb_timestamp;
	uint32_t         depth_timestamp;
	char             have_frames;
	freenect_raw_device_state* state;
	
} t_jit_freenect_grab;

typedef struct _obj_list
{
	t_jit_freenect_grab **objects;
	uint32_t count;
} t_obj_list;

void *_jit_freenect_grab_class;

<<<<<<< HEAD
t_jit_err 				jit_freenect_grab_init(void);
t_jit_freenect_grab		*jit_freenect_grab_new(void);
void 					jit_freenect_grab_free(t_jit_freenect_grab *x);
t_jit_err 				jit_freenect_grab_matrix_calc(t_jit_freenect_grab *x, void *inputs, void *outputs);
void					jit_freenect_open(t_jit_freenect_grab *x, t_symbol *s, long argc, t_atom *argv);
void					jit_freenect_close(t_jit_freenect_grab *x, t_symbol *s, long argc, t_atom *argv);
void					jit_freenect_tilt(t_jit_freenect_grab *x,  void *attr, long argc, t_atom *argv);
void					jit_freenect_accel(t_jit_freenect_grab *x,  t_symbol *s, long argc, t_atom *argv);

void					rgb_callback(freenect_device *dev, freenect_pixel *pixels, uint32_t timestamp);
void					depth_callback(freenect_device *dev, freenect_depth *pixels, uint32_t timestamp);
void					copy_depth_data(freenect_depth *source, char *out_bp, t_jit_matrix_info *dest_info);
void					copy_rgb_data(freenect_pixel *source, char *out_bp, t_jit_matrix_info *dest_info);
=======
t_jit_err               jit_freenect_grab_init(void);
t_jit_freenect_grab     *jit_freenect_grab_new(void);
void                    jit_freenect_grab_free(t_jit_freenect_grab *x);
>>>>>>> 9ac7675ae16b954ce114f9c144d6bb3806960962

void                    jit_freenect_open(t_jit_freenect_grab *x, t_symbol *s, long argc, t_atom *argv);
void                    jit_freenect_close(t_jit_freenect_grab *x, t_symbol *s, long argc, t_atom *argv);

t_jit_err               jit_freenect_grab_get_ndevices(t_jit_freenect_grab *x, void *attr, long *ac, t_atom **av);
t_jit_err               jit_freenect_grab_get_accel(t_jit_freenect_grab *x, void *attr, long *ac, t_atom **av);
void					jit_freenect_set_tilt(t_jit_freenect_grab *x,  void *attr, long argc, t_atom *argv);
t_jit_err				jit_freenect_get_tilt(t_jit_freenect_grab *x, void *attr, long *ac, t_atom **av);

t_jit_err               jit_freenect_set_mode(t_jit_freenect_grab *x, void *attr, long ac, t_atom *av);

t_jit_err               jit_freenect_grab_matrix_calc(t_jit_freenect_grab *x, void *inputs, void *outputs);
void                    copy_depth_data(freenect_depth *source, char *out_bp, t_jit_matrix_info *dest_info, t_lookup *lut);
void                    copy_rgb_data(freenect_pixel *source, char *out_bp, t_jit_matrix_info *dest_info);

void                    rgb_callback(freenect_device *dev, freenect_pixel *pixels, uint32_t timestamp);
void                    depth_callback(freenect_device *dev, void *pixels, uint32_t timestamp);

pthread_t       capture_thread;
int terminate_thread;

int object_count = 0;

<<<<<<< HEAD
void *capture_threadfunc(void *arg)
{
	int done = 0;
	freenect_context *f_ctx = NULL;
	freenect_device *f_dev = NULL;
	//freenect_device *f_dev = NULL;
	
	//printf("Entering grabber thread.\n");
	//post("Thread in.");
=======
freenect_context *f_ctx = NULL;
t_obj_list       object_list;

static int obj_list_push(t_obj_list *list, t_jit_freenect_grab *x){
	t_jit_freenect_grab **tmp;
>>>>>>> 9ac7675ae16b954ce114f9c144d6bb3806960962
	
	if(!list){
		error("obj_list_push: invalid pointer.");
		return -1;
	}
	
	tmp = (t_jit_freenect_grab **)realloc(list->objects, (list->count+1)*sizeof(t_jit_freenect_grab *));
	
	if(tmp){
		list->objects = tmp;
		list->objects[list->count] = x;
		list->count++;
	}
	else{
		error("list_push: Out of memory.");
		return -1;
	}
	
	return list->count - 1;
}

static int obj_list_remove_item(t_obj_list *list, t_jit_freenect_grab *x){
	int i;
	
	if(!list){
		error("obj_list_remove_item: invalid pointer.");
		return -1;
	}
	
	for(i=0;i<list->count;i++){
		if(list->objects[i] == x){
			for(i++;i<list->count;i++){
				list->objects[i-1] = list->objects[i];
			}
		}
		list->count--;
		list->objects[list->count] = NULL;
	}
	
	if(!list->count){
		free(list->objects);
		list->objects = NULL;
	}
	
<<<<<<< HEAD
	//freenect_set_led(f_dev,LED_GREEN);	//freenect_set_tilt_degs(f_dev,0);
	

	
	while(!done && freenect_process_events(f_ctx) >= 0){
		pthread_mutex_lock(&mutex);
		if(message.type == TERMINATE){
			done = 1;
			message.type = NONE;
=======
	return list->count;
}

void calculate_lut(t_lookup *lut, t_symbol *type, int mode){
	long i;
	
	if(type == _jit_sym_float32){
		float *f_lut;
		f_lut = (float *)realloc(lut->f_ptr, sizeof(float) * 0x800);
		if(!f_lut){
			error("Out of memory!");
			return;
		}
		lut->f_ptr = f_lut;
		
		switch(mode){
			case 0:
				for(i=0;i<0x800;i++){
					lut->f_ptr[i] = (float)i;
				}
				break;
			case 1:
				for(i=0;i<0x800;i++){
					lut->f_ptr[i] = (float)i * (1.f / (float)0x7FF);
				}
				break;
			case 2:
				for(i=0;i<0x800;i++){
					lut->f_ptr[i] = 1.f - ((float)i * (1.f / (float)0x7FF));
				}
				break;
			case 3:
				for(i=0;i<0x800;i++){
					lut->f_ptr[i] = 100.f / (3.33f + (float)i * -0.00307f);
				} 
				break;
>>>>>>> 9ac7675ae16b954ce114f9c144d6bb3806960962
		}
	}
	else if(type == _jit_sym_long){
		long *l_lut;
		l_lut = (long *)realloc(lut->l_ptr, sizeof(long) * 0x800);
		if(!l_lut){
			error("Out of memory!");
			return;
		}
		lut->l_ptr = l_lut;
		
		switch(mode){
			case 0:
			case 1:
				for(i=0;i<0x800;i++){
					lut->l_ptr[i] = i;
				}
				break;
			case 2:
				for(i=0;i<0x800;i++){
					lut->l_ptr[i] = 0x7FF - i;
				}
				break;
			case 3:
				for(i=0;i<0x800;i++){
					lut->l_ptr[i] = (long)(100.f / (3.33f + (float)i * -0.00307f));
				} 
				break;
		}
	}
	else if(type == _jit_sym_float64){
		double *d_lut;
		d_lut = (double *)realloc(lut->d_ptr, sizeof(double) * 0x800);
		if(!d_lut){
			error("Out of memory!");
			return;
		}
		lut->d_ptr = d_lut;
		
		switch(mode){
			case 0:
				for(i=0;i<0x800;i++){
					lut->d_ptr[i] = (double)i;
				}
				break;
			case 1:
				for(i=0;i<0x800;i++){
					lut->d_ptr[i] = (double)i * (1.0 / (double)0x7FF);
				}
				break;
			case 2:
				for(i=0;i<0x800;i++){
					lut->d_ptr[i] = 1.0 - ((double)i * (1.0 / (double)0x7FF));
				}
				break;
			case 3:
				for(i=0;i<0x800;i++){
					lut->d_ptr[i] = 100.0 / (3.33 + (double)i * -0.00307);
				} 
				break;
		}
	}
	else if(type == NULL){
		if(lut->f_ptr){
			free(lut->f_ptr);
			lut->f_ptr = NULL;
		}
	}
	else{
		error("Invalid type for lookup table calculation. char not supported.");
		return;
	}
}

void *capture_threadfunc(void *arg)
{
	int i;
	int process_events = 0;
	
	if(!f_ctx){
		if (freenect_init(&f_ctx, NULL) < 0) {
			printf("freenect_init() failed\n");
			goto out;
		}
	}
	
	terminate_thread = 0;
		
	while(!terminate_thread){
		process_events = 0;
		for(i=0;i<object_list.count;i++){
			if(object_list.objects[i]->device)
				process_events = 1;
			
			sleep(0);
		}
		
		if(process_events){
			if(freenect_process_events(f_ctx) < 0){
				error("Could not process events.");
				break;
			}
		}
	}
	
out:	
	pthread_exit(NULL);
	return NULL;
}

t_jit_err jit_freenect_grab_init(void)
{
	long attrflags=0;
	t_jit_object *attr;
	t_jit_object *mop,*output;
<<<<<<< HEAD
	t_atom a[1];

=======
	t_atom a[4];
	
>>>>>>> 9ac7675ae16b954ce114f9c144d6bb3806960962
	_jit_freenect_grab_class = jit_class_new("jit_freenect_grab",(method)jit_freenect_grab_new,(method)jit_freenect_grab_free, sizeof(t_jit_freenect_grab),0L);
  	
	//add mop
	mop = (t_jit_object *)jit_object_new(_jit_sym_jit_mop,0,2); //0 inputs, 2 outputs
	
	//Prepare depth image, all values are hard-coded, may need to be queried for safety?
	output = jit_object_method(mop,_jit_sym_getoutput,1);
		
	jit_atom_setsym(a,_jit_sym_float32); //default
	jit_atom_setsym(a+1,_jit_sym_long);
	jit_atom_setsym(a+2,_jit_sym_float64);
	jit_object_method(output,_jit_sym_types,3,a);
	
	jit_attr_setlong(output,_jit_sym_minplanecount,1);
	jit_attr_setlong(output,_jit_sym_maxplanecount,1);
	
	jit_atom_setlong(&a[0], DEPTH_WIDTH);
	jit_atom_setlong(&a[1], DEPTH_HEIGHT);
	
	jit_object_method(output, _jit_sym_mindim, 2, a);  //Two dimensions, sizes in atom array
	jit_object_method(output, _jit_sym_maxdim, 2, a);
	
	//Prepare RGB image
	output = jit_object_method(mop,_jit_sym_getoutput,2);
	
	jit_atom_setsym(a,_jit_sym_char); //default
	jit_object_method(output,_jit_sym_types,1,a);
	
	jit_attr_setlong(output,_jit_sym_minplanecount,4);
	jit_attr_setlong(output,_jit_sym_maxplanecount,4);
	
	jit_atom_setlong(&a[0], RGB_WIDTH);
	jit_atom_setlong(&a[1], RGB_HEIGHT);
	
	jit_object_method(output, _jit_sym_mindim, 2, a);
	jit_object_method(output, _jit_sym_maxdim, 2, a);
	
	jit_class_addadornment(_jit_freenect_grab_class,mop);
	
	//add methods
	jit_class_addmethod(_jit_freenect_grab_class, (method)jit_freenect_open, "open", A_GIMME, 0L);
	jit_class_addmethod(_jit_freenect_grab_class, (method)jit_freenect_close, "close", A_GIMME, 0L);
	jit_class_addmethod(_jit_freenect_grab_class, (method)jit_freenect_accel, "accel", A_GIMME, 0L);

	
	jit_class_addmethod(_jit_freenect_grab_class, (method)jit_freenect_grab_matrix_calc, "matrix_calc", A_CANT, 0L);
	
	//add attributes	
	attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_USURP_LOW;
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,"unique",_jit_sym_char,
										  attrflags,(method)NULL,(method)NULL,calcoffset(t_jit_freenect_grab,unique));
	jit_attr_addfilterset_clip(attr,0,1,TRUE,TRUE);
	jit_class_addattr(_jit_freenect_grab_class,attr);
	
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,"mode",_jit_sym_char,
										  attrflags,(method)NULL,(method)jit_freenect_set_mode,calcoffset(t_jit_freenect_grab,mode));
	jit_class_addattr(_jit_freenect_grab_class,attr);
	
<<<<<<< HEAD
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,"tilt",_jit_sym_long,attrflags,(method)NULL,(method)jit_freenect_tilt,calcoffset(t_jit_freenect_grab,tilt));
	jit_attr_addfilterset_clip(attr,-30,30,TRUE,TRUE);
	jit_class_addattr(_jit_freenect_grab_class,attr);
	
	//attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_OPAQUE;
	
	

	jit_class_addmethod(_jit_freenect_grab_class, (method)jit_object_register, 	"register",			A_CANT, 0L); // can register
	
	jit_class_register(_jit_freenect_grab_class);
=======
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,"tilt",_jit_sym_float64,
										  attrflags,(method)jit_freenect_get_tilt,(method)jit_freenect_set_tilt,calcoffset(t_jit_freenect_grab,tilt));
	jit_class_addattr(_jit_freenect_grab_class,attr);
>>>>>>> 9ac7675ae16b954ce114f9c144d6bb3806960962
	
	attrflags = JIT_ATTR_GET_DEFER_LOW | JIT_ATTR_SET_OPAQUE;
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,"index",_jit_sym_long,
										  attrflags,(method)NULL,(method)NULL,calcoffset(t_jit_freenect_grab,index));
	jit_class_addattr(_jit_freenect_grab_class,attr);
	
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,"ndevices",_jit_sym_long,
										  attrflags,(method)jit_freenect_grab_get_ndevices,(method)NULL,calcoffset(t_jit_freenect_grab,ndevices));
	jit_class_addattr(_jit_freenect_grab_class,attr);
	
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset_array, "accel", _jit_sym_float64, 3, 
										  attrflags, (method)jit_freenect_grab_get_accel,(method)NULL, 
										  calcoffset(t_jit_freenect_grab, accelcount),calcoffset(t_jit_freenect_grab,mks_accel));
	jit_class_addattr(_jit_freenect_grab_class,attr);
	
<<<<<<< HEAD
	if (pthread_create(&capture_thread, NULL, capture_threadfunc, NULL)) {
		error("Failed to create capture thread.\n");
		return JIT_ERR_GENERIC;
	}
 
=======
	attrflags = JIT_ATTR_GET_OPAQUE_USER | JIT_ATTR_SET_OPAQUE_USER;
	attr = (t_jit_object *)jit_object_new(_jit_sym_jit_attr_offset,"has_frames",_jit_sym_char,
										  attrflags,(method)NULL,(method)NULL,calcoffset(t_jit_freenect_grab,has_frames));
	jit_class_addattr(_jit_freenect_grab_class,attr);
	
	jit_class_register(_jit_freenect_grab_class);
		
	object_list.objects = NULL;
	object_list.count = 0;
	
>>>>>>> 9ac7675ae16b954ce114f9c144d6bb3806960962
	return JIT_ERR_NONE;
}

t_jit_freenect_grab *jit_freenect_grab_new(void)
{
	t_jit_freenect_grab *x;
	
	if (x=(t_jit_freenect_grab *)jit_object_alloc(_jit_freenect_grab_class))
	{
		x->device = NULL;
		x->timestamp = 0;
		x->unique = 0;
		x->mode = 0;
		x->has_frames = 0;
		x->ndevices = 0;
		x->lut.f_ptr = NULL;
		x->lut_type = NULL;
		x->tilt = 0;
		
		if(object_list.count == 0){
			if (pthread_create(&capture_thread, NULL, capture_threadfunc, NULL)) {
				error("Failed to create capture thread.");
				return NULL;
			}
		}
		
<<<<<<< HEAD
		object_count++;
		post("Object count: %d", object_count);
		

=======
		obj_list_push(&object_list, x);
>>>>>>> 9ac7675ae16b954ce114f9c144d6bb3806960962
		
	} else {
		x = NULL;
	}	
	return x;
}

void jit_freenect_grab_free(t_jit_freenect_grab *x)
{
	jit_freenect_close(x, NULL, 0, NULL);
		
	obj_list_remove_item(&object_list, x);
	
	if(x->lut.f_ptr){
		free(x->lut.f_ptr);
	}
	
	if(object_list.count == 0){
		terminate_thread = 1;
	}
}

t_jit_err jit_freenect_grab_get_ndevices(t_jit_freenect_grab *x, void *attr, long *ac, t_atom **av){
	
	if ((*ac)&&(*av)) {
		
	} else {
		*ac = 1;
		if (!(*av = jit_getbytes(sizeof(t_atom)*(*ac)))) {
			*ac = 0;
			return JIT_ERR_OUT_OF_MEM;
		}
	}
	
	if(f_ctx){
		x->ndevices = freenect_num_devices(f_ctx);
	}
	else{
		x->ndevices = 0;
	}
	
	
	jit_atom_setlong(*av,x->ndevices);
	
	return JIT_ERR_NONE;
}

<<<<<<< HEAD
void jit_freenect_accel(t_jit_freenect_grab *x, t_symbol *s, long argc, t_atom *argv)
{
	
	// mode 0 - raw, 1 - mks
	int mode=0;
	
	if (jit_atom_getsym(argv)==gensym("mks")) mode=1;
	
		t_atom acc[3];
	
	
	int i;
	if (x->device){
		for(i=0;i<MAX_DEVICES;i++){
			if(device_data[i].device == x->device){
				
				if(x->device){
					
					if (mode==0){
						int16_t ax,ay,az;
						freenect_get_raw_accel(x->device, &ax, &ay, &az);
						
						jit_atom_setlong(&acc[0],ax);
						jit_atom_setlong(&acc[1],ay);
						jit_atom_setlong(&acc[2],az);
						
						jit_object_notify(x,gensym("acc_raw"), acc);
						
					} else {
					
						double dx,dy,dz;
						freenect_get_mks_accel(x->device, &dx, &dy, &dz);
						
						jit_atom_setfloat(&acc[0],dx);
						jit_atom_setfloat(&acc[1],dy);
						jit_atom_setfloat(&acc[2],dz);
						
						jit_object_notify(x,gensym("acc_mks"), acc);
					
					}

					
					break;
				}
				
				
				
			}
		}
	} else jit_object_post((t_object *)x,"accel_raw:invalid device");
			
}


void jit_freenect_tilt(t_jit_freenect_grab *x,  void *attr, long argc, t_atom *argv)
{
	
	long tilt=0;
	
	if (argv)
	{
	
		
		tilt = jit_atom_getlong(argv);

		x->tilt = tilt;
			
		int i;
		for(i=0;i<MAX_DEVICES;i++){
			if(device_data[i].device == x->device){
				
				if(x->device)
				freenect_set_tilt_degs(x->device,x->tilt);
				else jit_object_post((t_object *)x,"tilt:invalid device");
				break;
			}
		}
		 
		

	}
	
}

void jit_freenect_open(t_jit_freenect_grab *x,  t_symbol *s, long argc, t_atom *argv)
{
		//TODO, close/start is automatic on object destruction/creation for now,  jmp 2010/11/21
		
	//TODO: remove this hack on merge with jmp
	x->device = device_data[0].device;

=======
t_jit_err jit_freenect_grab_get_accel(t_jit_freenect_grab *x, void *attr, long *ac, t_atom **av){
	double ax=0,ay=0,az=0;


	if ((*ac)&&(*av)) {
		
	} else {
		*ac = 3;
		if (!(*av = jit_getbytes(sizeof(t_atom)*(*ac)))) {
			*ac = 0;
			return JIT_ERR_OUT_OF_MEM;
		}
	}

	if(x->device){
		freenect_update_device_state(x->device);
		x->state = freenect_get_device_state(x->device);
		if (x->state)
			freenect_get_mks_accel(x->state, &ax, &ay, &az);
		
	}
	
	jit_atom_setfloat(*av, ax);
	jit_atom_setfloat(*av +1, ay);
	jit_atom_setfloat(*av +2, az);
		
	return JIT_ERR_NONE;
}

t_jit_err jit_freenect_set_mode(t_jit_freenect_grab *x, void *attr, long ac, t_atom *av){
    if(ac < 1){
        return JIT_ERR_NONE;
    }
	
	if(x->mode != jit_atom_getlong(av)){
		x->mode = jit_atom_getlong(av);
		CLIP(x->mode, 0, 3);
		calculate_lut(&x->lut, x->lut_type, x->mode);
	}
	
    return JIT_ERR_NONE;
}

void jit_freenect_set_tilt(t_jit_freenect_grab *x,  void *attr, long argc, t_atom *argv)
{
	if(argv){
		x->tilt = jit_atom_getfloat(argv);

		CLIP(x->tilt, -30, 30);
		
		if(x->device){
			freenect_set_tilt_degs(x->device,x->tilt);
		}
	}
}
t_jit_err jit_freenect_get_tilt(t_jit_freenect_grab *x, void *attr, long *ac, t_atom **av){
	double tilt=0;
	
	
	if ((*ac)&&(*av)) {
		
	} else {
		*ac = 1;
		if (!(*av = jit_getbytes(sizeof(t_atom)*(*ac)))) {
			*ac = 0;
			return JIT_ERR_OUT_OF_MEM;
		}
	}
	
	if(x->device){
		freenect_update_device_state(x->device);
		x->state = freenect_get_device_state(x->device);
		if (x->state)
			tilt = freenect_get_tilt_degs(x->state);
		
	}
	
	jit_atom_setfloat(*av, tilt);

	
	return JIT_ERR_NONE;
}


void jit_freenect_open(t_jit_freenect_grab *x,  t_symbol *s, long argc, t_atom *argv)
{
	int ndevices, i, devices_left, dev_ndx;
	if(!f_ctx){
		error("Invalid context!");
		return;
	}
	
	if(x->device){
		post("A device is already open.");
		return;
	}
	
	ndevices = freenect_num_devices(f_ctx);
	
	if(!ndevices){
		post("Could not find any connected Kinect device. Are you sure the power cord is plugged-in?");
		return;
	}
	
	devices_left = ndevices;
	
	for(i=0;i<object_list.count;i++){
		if(object_list.objects[i]->device)
			devices_left--;
	}
	if(!devices_left){
		post("All Kinect devices are currently in use.");
		return;
	}
	
	if(!argc){
		x->index = 0;	
	}
	else{
		//Is the device already in use?
		x->index = jit_atom_getlong(argv);
		
		for(i=0;i<object_list.count;i++){
			if(object_list.objects[i]->device){
				if(object_list.objects[i]->index == x->index){
					post("Kinect device %d is already in use.", x->index);
					x->index = 0;
					return;
				}
			}
		}
	}
	
	if(ndevices <= x->index){
		post("Cannot open Kinect device %d, only % are connected.", x->index, ndevices);
		x->index = 0;
		return;
	}
	
	//Find out which device to open
	dev_ndx = x->index;
	if(!dev_ndx){
		int found = 0;
		while(!found){
			found = 1;
			for(i=0;i<object_list.count;i++){
				if(object_list.objects[i]->device){
					if(object_list.objects[i]->index-1 == dev_ndx){
						found = 0;
						break;
					}
				}
			}
			dev_ndx++;
		}
		x->index = dev_ndx;
	}
	
	post("Opening Kinect device %d", dev_ndx);
	
	if (freenect_open_device(f_ctx, &(x->device), dev_ndx-1) < 0) {
		error("Could not open Kinect device %d", dev_ndx);
		x->index = 0;
		x->device = NULL;
	}
	
	freenect_set_depth_callback(x->device, depth_callback);
	freenect_set_rgb_callback(x->device, rgb_callback);
	freenect_set_rgb_format(x->device, FREENECT_FORMAT_RGB);
	
	//Store a pointer to this object in the freenect device struct (for use in callbacks)
	freenect_set_user(x->device, x);  
	
	freenect_set_led(x->device,LED_RED);
	
	freenect_set_tilt_degs(x->device,x->tilt);
	
	freenect_start_depth(x->device);
	freenect_start_rgb(x->device);
>>>>>>> 9ac7675ae16b954ce114f9c144d6bb3806960962
}

void jit_freenect_close(t_jit_freenect_grab *x,  t_symbol *s, long argc, t_atom *argv)
{
<<<<<<< HEAD


	
	/*
	freenect_angle--;
	if (freenect_angle < -30) {
		freenect_angle = -30;
	}
	freenect_set_tilt_degs(x->device,freenect_angle);
	*/
	//TODO, close/start is automatic on object destruction/creation for now,  jmp 2010/11/21
=======
	if(!x->device)return;
	post("Closing Kinect device %d", x->index);
	freenect_set_led(x->device,LED_BLINK_GREEN);
	freenect_close_device(x->device);
	x->device = NULL;
>>>>>>> 9ac7675ae16b954ce114f9c144d6bb3806960962
}

t_jit_err jit_freenect_grab_matrix_calc(t_jit_freenect_grab *x, void *inputs, void *outputs)
{
	t_jit_err err=JIT_ERR_NONE;
	long depth_savelock=0,rgb_savelock=0;
	t_jit_matrix_info depth_minfo,rgb_minfo;
	void *depth_matrix,*rgb_matrix;
	char *depth_bp, *rgb_bp;
			
	depth_matrix = jit_object_method(outputs,_jit_sym_getindex,0); 
	rgb_matrix = jit_object_method(outputs,_jit_sym_getindex,1); 
	
	if (x && depth_matrix && rgb_matrix) {
		
		depth_savelock = (long) jit_object_method(depth_matrix,_jit_sym_lock,1);
		rgb_savelock = (long) jit_object_method(rgb_matrix,_jit_sym_lock,1);
		
		if(!x->device){
			goto out;
		}
		
		jit_object_method(depth_matrix,_jit_sym_getinfo,&depth_minfo);
		jit_object_method(rgb_matrix,_jit_sym_getinfo,&rgb_minfo);
		
		if ((depth_minfo.type == _jit_sym_char) || (rgb_minfo.type != _jit_sym_char)) 
		{
			err=JIT_ERR_MISMATCH_TYPE;
			goto out;
		}
		
		if ((depth_minfo.planecount != 1) || (rgb_minfo.planecount != 4)) //overkill, but you can never be too sure
		{
			err=JIT_ERR_MISMATCH_PLANE;
			goto out;
		}
		
		if ((depth_minfo.dim[0] != DEPTH_WIDTH) || (depth_minfo.dim[1] != DEPTH_HEIGHT) ||
			(rgb_minfo.dim[0] != RGB_WIDTH) || (rgb_minfo.dim[1] != RGB_HEIGHT)) //overkill, but you can never be too sure
		{
			err=JIT_ERR_MISMATCH_DIM;
			goto out;
		}
		
		if ((depth_minfo.dimcount != 2) || (rgb_minfo.dimcount != 2)) //overkill, but you can never be too sure
		{
			err=JIT_ERR_MISMATCH_DIM;
			goto out;
		}
		
		jit_object_method(depth_matrix,_jit_sym_getdata,&depth_bp);
		if (!depth_bp) { err=JIT_ERR_INVALID_OUTPUT; goto out;}
		
		jit_object_method(rgb_matrix,_jit_sym_getdata,&rgb_bp);
		if (!rgb_bp) { err=JIT_ERR_INVALID_OUTPUT; goto out;}
		
		if((depth_minfo.type != x->lut_type) || !x->lut.f_ptr){
			calculate_lut(&x->lut, depth_minfo.type, x->mode);
			x->lut_type = depth_minfo.type;
		}
		 
		//Grab and copy matrices
		x->has_frames = 0;  //Assume there are no new frames
		
		if(x->have_frames > 1){ //2 or more new frames: depth and rgb
			x->have_frames = 0;			
			x->timestamp = MAX(x->rgb_timestamp,x->depth_timestamp);
			x->has_frames = 1; //We have new frames to output in "unique" mode
			
			copy_depth_data(x->depth_data, depth_bp, &depth_minfo, &x->lut);
			copy_rgb_data(x->rgb_data, rgb_bp, &rgb_minfo);
		}
		
	} else {
		return JIT_ERR_INVALID_PTR;
	}
	
out:
	jit_object_method(depth_matrix,gensym("lock"),depth_savelock);
	jit_object_method(rgb_matrix,gensym("lock"),rgb_savelock);
	return err;
}

void copy_depth_data(freenect_depth *source, char *out_bp, t_jit_matrix_info *dest_info, t_lookup *lut)
{
	int i,j;
	freenect_depth *in;
	
	if(!source){
		return;	
	}
	
	if(!out_bp || !dest_info){
		error("Invalid pointer in copy_depth_data.");
		return;
	}
	
	in = source;
	
	if(dest_info->type == _jit_sym_float32){
		float *out;
		for(i=0;i<DEPTH_HEIGHT;i++){
			out = (float *)(out_bp + dest_info->dimstride[1] * i);
			for(j=0;j<DEPTH_WIDTH;j++){
				out[j] = lut->f_ptr[*in];
				in++;
			}
		}
	}
	else if(dest_info->type == _jit_sym_float64){
		double *out;
		for(i=0;i<DEPTH_HEIGHT;i++){
			out = (double *)(out_bp + dest_info->dimstride[1] * i);
			for(j=0;j<DEPTH_WIDTH;j++){
				out[j] = lut->d_ptr[*in];
				in++;
			}
		}
	}
	else if(dest_info->type == _jit_sym_long){
		long *out;
		for(i=0;i<DEPTH_HEIGHT;i++){
			out = (long *)(out_bp + dest_info->dimstride[1] * i);
			for(j=0;j<DEPTH_WIDTH;j++){
				out[j] = lut->l_ptr[*in];
				in++;
			}
		}
	}
}

void copy_rgb_data(freenect_pixel *source, char *out_bp, t_jit_matrix_info *dest_info)
{
	int i,j;
	
	char *out;
	freenect_pixel *in;
	
	if(!source){
		return;
	}
	
	if(!out_bp || !dest_info){
		error("Invalid pointer in copy_depth_data.");
		return;
	}
	
	in = source;
	
	for(i=0;i<RGB_HEIGHT;i++){
		out = out_bp + dest_info->dimstride[1] * i;
		for(j=0;j<RGB_WIDTH;j++){
			out[0] = 0xFF;
			out[1] = in[0];
			out[2] = in[1];
			out[3] = in[2];
			
			out += 4;
			in += 3;
		}
	}
}

void rgb_callback(freenect_device *dev, freenect_pixel *pixels, uint32_t timestamp){
	t_jit_freenect_grab *x;
	
	x = freenect_get_user(dev);
	
	if(!x)return;
	
	x->rgb_data = pixels;
	x->rgb_timestamp = timestamp;
	x->have_frames++;
}

void depth_callback(freenect_device *dev, void *pixels, uint32_t timestamp){
	t_jit_freenect_grab *x;
	
	x = freenect_get_user(dev);
	
	if(!x)return;
	
	x->depth_data = pixels;
	x->depth_timestamp = timestamp;
	x->have_frames++;
}
