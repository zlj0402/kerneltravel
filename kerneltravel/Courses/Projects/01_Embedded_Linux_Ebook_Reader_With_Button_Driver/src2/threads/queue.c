#include <pthread.h>
#include <stdio.h>

#include <queue.h>
#define KEY_BUF_SIZE 128

static char g_KeyBuf[KEY_BUF_SIZE];
static int g_iKeyHead = 0;
static int g_iKeyTail = 0;
static pthread_mutex_t g_KeyMutex = PTHREAD_MUTEX_INITIALIZER;

void EnqueueKey(char cKey) {
	
    pthread_mutex_lock(&g_KeyMutex);
    int iNext = (g_iKeyHead + 1) % KEY_BUF_SIZE;
    if (iNext != g_iKeyTail) { // not full

        g_KeyBuf[g_iKeyHead] = cKey;
        g_iKeyHead = iNext;
    } else {
        // 队列满，丢弃
        fprintf(stderr, "[queue] warning: key queue full, dropping '%c'\n", cKey);
    }
    pthread_mutex_unlock(&g_KeyMutex);
}

int DequeueKey(char *pcKey) {
	
    pthread_mutex_lock(&g_KeyMutex);
    if (g_iKeyHead == g_iKeyTail) {

        pthread_mutex_unlock(&g_KeyMutex);
        return 0; // empty
    }
    *pcKey = g_KeyBuf[g_iKeyTail];
    g_iKeyTail = (g_iKeyTail + 1) % KEY_BUF_SIZE;
    pthread_mutex_unlock(&g_KeyMutex);
	
    return 1;
}
