/*
 *  Task.h
 *  framework
 *
 *  Created by Johnny Chan on 10/12/09.
 *  Copyright 2009 Llamaface. All rights reserved.
 *
 */

#ifndef __TASK_H__
#define __TASK_H__

#include "types.h"


class CTask
{
private:
	int _start_time;
	int _end_time;
	int _duration;
	int _time_passed;
	BOOL _is_done;

public:
	void init(int duration);
	void startTask(void);
	void setDuration(void);
	void pauseTask(void);
	inline BOOL isDone(void) {return _is_done;}	
	
};


#endif