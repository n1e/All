<<<<<<< HEAD
#include <pthread.h>

#ifdef _WIN32
#define ATOM_CMP_SET(lock,old,new)\
	(InterlockedCompareExchange(lock,new,old) == old)

#define SCHED_YIELD()\
	SwitchToThread()


#else
#define ATOM_CMP_SET(lock,old,new)\
	(__sync_bool_compare_and_swap(lock,old,new))

#define SCHED_YIELD()\
	sched_yield()
#endif



#define TRY_LOOP(nCount)\
	for (int i = 1 ; i < nCount ; i<<=1 )\
	{\
		for (int j = 0 ; j < i ; j++ )\
		{\
			CPU_PAUSE();\
		}

#define END_LOOP()\
	}\
	SCHED_YIELD();


typedef volatile uint32_t atomic_t;
typedef RWLOCK_WLOCK ((uint32_t) -1);


void ADDWLock(atomic_t* lock)
{
	for ( ; ; )
	{
		if (*lock == 0 && ATOM_CMP_SET(lock, 0, RWLOCK_WLOCK))
			return;
		else
		{
			TRY_LOOP()
			if (*lock == 0 && ATOM_CMP_SET(lock, 0, RWLOCK_WLOCK))
				return;
			END_LOOP()
		}
	}
}

void ADDRLock(atomic_t* lock)
{
	atomic_t reader;
	for ( ;; )
	{
		reader = *lock;
		if (reader != RWLOCK_WLOCK &&  ATOM_CMP_SET(lock, reader, reader + 1))
			return;
		else
		{
			TRY_LOOP()
			reader = *lock;
			if (reader != RWLOCK_WLOCK && ATOM_CMP_SET(lock, reader, reader + 1))
				return;
			END_LOOP()
		}
		
	}
}

void UnLock(atomic_t* lock)
{
	atomic_t reader = *lock;
	if (*lock = RWLOCK_WLOCK)
	{
		*lock = 0;
		return;
	}
	else
	{
		for ( ;; )
		{
			if (ATOM_CMP_SET(lock, reader, reader - 1))
				return;

			reader = *lock;//����?12�顤��?a?��??��?����o��,?a???�䨦��D�� ?��reader = *locko����?��D������?????3��?a��??��??,??��?��??a??��??TD���?
		}
	}
}
=======
#include <pthread.h>

#ifdef _WIN32
#define ATOM_CMP_SET(lock,old,new)\
	(InterlockedCompareExchange(lock,new,old) == old)

#define SCHED_YIELD()\
	SwitchToThread()


#else
#define ATOM_CMP_SET(lock,old,new)\
	(__sync_bool_compare_and_swap(lock,old,new))

#define SCHED_YIELD()\
	sched_yield()
#endif



#define TRY_LOOP(nCount)\
	for (int i = 1 ; i < nCount ; i<<=1 )\
	{\
		for (int j = 0 ; j < i ; j++ )\
		{\
			CPU_PAUSE();\
		}

#define END_LOOP()\
	}\
	SCHED_YIELD();


typedef volatile uint32_t atomic_t;
typedef RWLOCK_WLOCK ((uint32_t) -1);


void ADDWLock(atomic_t* lock)
{
	for ( ; ; )
	{
		if (*lock == 0 && ATOM_CMP_SET(lock, 0, RWLOCK_WLOCK))
			return;
		else
		{
			TRY_LOOP(1024)
			if (*lock == 0 && ATOM_CMP_SET(lock, 0, RWLOCK_WLOCK))
				return;
			END_LOOP()
		}
	}
}

void ADDRLock(atomic_t* lock)
{
	atomic_t reader;
	for ( ;; )
	{
		reader = *lock;
		if (reader != RWLOCK_WLOCK &&  ATOM_CMP_SET(lock, reader, reader + 1))
			return;
		else
		{
			TRY_LOOP(1024)
			reader = *lock;
			if (reader != RWLOCK_WLOCK && ATOM_CMP_SET(lock, reader, reader + 1))
				return;
			END_LOOP()
		}
		
	}
}

void UnLock(atomic_t* lock)
{
	atomic_t reader = *lock;
	if (*lock = RWLOCK_WLOCK)
	{
		*lock = 0;
		return;
	}
	else
	{
		for ( ;; )
		{
			if (ATOM_CMP_SET(lock, reader, reader - 1))
				return;

			reader = *lock;//��ֹ�����������ʱ��,����δ��Ч ��reader = *lock������һ���߳̽��˶���,�Ǳ��ν�������Ч��
		}
	}
}
>>>>>>> 01f80ca41fe83558defb10844053c7ae939f3f21
