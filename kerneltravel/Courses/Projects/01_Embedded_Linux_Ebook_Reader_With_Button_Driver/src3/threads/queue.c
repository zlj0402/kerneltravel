#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#include <config.h>
#include <queue.h>
#define KEY_BUF_SIZE 128
#define NEXT_POS(x) ((x+1)%KEY_BUF_SIZE)
#define ONE_DAY_JIFFIES (24U * 60U * 60U * 100U)

#define QUIT_THRESHOLD 20

static int g_aiKeyBuf[KEY_BUF_SIZE];
static int g_iKeyHead = 0;
static int g_iKeyTail = 0;
static pthread_mutex_t g_KeyMutex = PTHREAD_MUTEX_INITIALIZER;

void EnqueueKey(int iKey) {
	
    pthread_mutex_lock(&g_KeyMutex);
    int iNext = NEXT_POS(g_iKeyHead);
    if (iNext != g_iKeyTail) { // not full

        g_aiKeyBuf[g_iKeyHead] = iKey;
        g_iKeyHead = iNext;
    } else {
        // 队列满，丢弃
        fprintf(stderr, "[queue] warning: key queue full, dropping '%c'\n", iKey);
    }
    pthread_mutex_unlock(&g_KeyMutex);
}

/**
 * @brief: 在 DequeueKey() 中引入短暂延迟再检查, 避免 diff <= QUIT_THRESHOLD，处理时，队列只有一个元素
 * @retval 0: empty queue
 * @retval 1/2: dequeue 单键或双键
 */
int DequeueKey(char *pcKey) {
	
	pthread_mutex_lock(&g_KeyMutex);
    if (g_iKeyTail == g_iKeyHead) {
		
        pthread_mutex_unlock(&g_KeyMutex);
        return 0; // empty
    }

    int iNextTail = NEXT_POS(g_iKeyTail);
    if (iNextTail == g_iKeyHead) {
        // 队列只有一个元素，可能另一个还没来
        pthread_mutex_unlock(&g_KeyMutex);
        usleep(QUIT_THRESHOLD * 1000); // 等待20ms再检查（根据 QUIT_THRESHOLD 设定）
        pthread_mutex_lock(&g_KeyMutex);

        iNextTail = NEXT_POS(g_iKeyTail);
        if (iNextTail == g_iKeyHead) {
            // 仍然只有一个，确实是单键
            *pcKey = g_aiKeyBuf[g_iKeyTail] & 0b1 ? 'u' : 'n';
            g_iKeyTail = iNextTail;
            pthread_mutex_unlock(&g_KeyMutex);
            return 1;
        }
        // 如果等待后多了一个键，继续双键逻辑
    }
	
	// 以下保持原双键检测逻辑
    if (((g_aiKeyBuf[g_iKeyTail] | g_aiKeyBuf[iNextTail]) & 0b11) == 0b11) {
		
        unsigned int iTailTime = g_aiKeyBuf[g_iKeyTail] >> 8;
        unsigned int iNextTailTime = g_aiKeyBuf[iNextTail] >> 8;
        unsigned int diff = (iNextTailTime >= iTailTime)
                            ? (iNextTailTime - iTailTime)
                            : (iNextTailTime + ONE_DAY_JIFFIES - iTailTime);
		if (DEBUG) {
				printf("tail time: %u, next tail time: %u, time diff: %u\n", iTailTime, iNextTailTime, diff);
		}
        if (diff <= QUIT_THRESHOLD) {
			
            *pcKey = 'q';
            g_iKeyTail = NEXT_POS(iNextTail);
            pthread_mutex_unlock(&g_KeyMutex);
			if (DEBUG) {
				printf("will quit: %u\n\n", diff);
			}
            return 2;
        }
    }

    *pcKey = g_aiKeyBuf[g_iKeyTail] & 0b1 ? 'u' : 'n';
    g_iKeyTail = iNextTail;
    pthread_mutex_unlock(&g_KeyMutex);
    return 1;
}

/**
 * @retval 0: empty queue
 * @retval 1/2: dequeue 单键或双键
 */
 /*
int DequeueKey(char *pcKey) {
	
    pthread_mutex_lock(&g_KeyMutex);
    if (g_iKeyTail == g_iKeyHead) {

        pthread_mutex_unlock(&g_KeyMutex);
        return 0; // empty
    }
	
	int iNextTail = NEXT_POS(g_iKeyTail);
	// 处理组合键
	if (iNextTail != g_iKeyHead) {
		
		// 目前只有两个按键，读过来键值，实际只有低两位判断哪个按键
		// 第 2 位 -> 下一页; 第 1 位 -> 上一页;
		if ( ( (g_aiKeyBuf[g_iKeyTail] | g_aiKeyBuf[iNextTail]) & 0b11 ) == 0b11 ) {
			
			// 单位: 1 jiffy
			unsigned int iTailTime = g_aiKeyBuf[g_iKeyTail] >> 8;
			unsigned int iNextTailTime = g_aiKeyBuf[iNextTail] >> 8;
			unsigned int diff = (iNextTailTime >= iTailTime)
						? (iNextTailTime - iTailTime)
						: (iNextTailTime + ONE_DAY_JIFFIES - iTailTime);
			
			if (DEBUG) {
				printf("tail time: %u, next tail time: %u, time diff: %u\n", iTailTime, iNextTailTime, diff);
			}
			// 50 ms 内的双键理解为 q
			if (diff <= QUIT_THRESHOLD) {
				
				*pcKey = 'q';
				g_iKeyTail = NEXT_POS(iNextTail);
				pthread_mutex_unlock(&g_KeyMutex);
				if (DEBUG) {
					printf("will quit: %u\n\n", diff);
				}
				return 2;
			}
		}
	}
	// 单键处理
	*pcKey = g_aiKeyBuf[g_iKeyTail] & 0b1 ? 'u' : 'n';
	g_iKeyTail = iNextTail;
	
	pthread_mutex_unlock(&g_KeyMutex);
	
    return 1;
}
*/
