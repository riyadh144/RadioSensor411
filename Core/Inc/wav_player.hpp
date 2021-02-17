//Special thanks to Mohamed Yaqoob I am using his code here.

#ifndef WAV_PLAYER_HPP
#define WAV_PLAYER_HPP

#include "stdint.h"
#include "i2s.hpp"
#include "fatfs.h"

class wav_player
{
    private:
    i2s i2sx;
    //WAV File System variables
    FIL wavFile;

    //WAV Audio Buffer
    uint32_t fileLength;
    #define AUDIO_BUFFER_SIZE  4096*1
    uint8_t audioBuffer[AUDIO_BUFFER_SIZE];
    __IO uint32_t audioRemainSize = 0;

    //WAV Player
    uint32_t samplingFreq;
    UINT playerReadBytes = 0;
    uint8_t isFinished = 0;

    //WAV Player process states
    typedef enum
    {
    PLAYER_CONTROL_Idle=0,
    PLAYER_CONTROL_HalfBuffer,
    PLAYER_CONTROL_FullBuffer,
    PLAYER_CONTROL_EndOfFile,
    }PLAYER_CONTROL_e;
    volatile PLAYER_CONTROL_e playerControlSM = PLAYER_CONTROL_Idle;

    //Audio Buffer State
    typedef enum
    {
    BUFFER_OFFSET_NONE = 0,
    BUFFER_OFFSET_HALF,
    BUFFER_OFFSET_FULL,
    }BUFFER_StateTypeDef;

    typedef enum
    {
    PLAY_Idle=0,
    PLAY_Pause,
    PLAY_Resume,
    }PLAY_State_e;

    typedef struct
    {
    uint32_t   ChunkID;       /* 0 */
    uint32_t   FileSize;      /* 4 */
    uint32_t   FileFormat;    /* 8 */
    uint32_t   SubChunk1ID;   /* 12 */
    uint32_t   SubChunk1Size; /* 16*/
    uint16_t   AudioFormat;   /* 20 */
    uint16_t   NbrChannels;   /* 22 */
    uint32_t   SampleRate;    /* 24 */

    uint32_t   ByteRate;      /* 28 */
    uint16_t   BlockAlign;    /* 32 */
    uint16_t   BitPerSample;  /* 34 */
    uint32_t   SubChunk2ID;   /* 36 */
    uint32_t   SubChunk2Size; /* 40 */

    }WAV_HeaderTypeDef;

    public:
    
    /**
     * @brief Construct a new wave player object
     * 
     */
    wav_player(i2s::i2s_num i2snum);
    /**
     * @brief initialize i2s interface
     * 
     */
    void init();
    /**
     * @brief Select WAV file to play
     * @retval 1: when file is found 0 when not
     */     
    uint8_t file_select(char * filePath);

    /**
     * @brief WAV File Play
     */
    void play(void);

    /**
     * @brief WAV stop
     */
    void stop(void);

    /**
     * @brief Process WAV
     */
    void process(void);

    /**
     * @brief isEndofFile reached
     */
    bool isEndOfFile(void);

    /**
     * @brief WAV pause/resume
     */
    void pause(void);
    void resume(void);
    /**
     * @brief reset the player
     * 
     */
    void reset(void);

    //sadly for now those will need to be called from the callback
    
    /**
     * @brief complete transfer callback function
     * @note sadly they will need to be handled from the callback
     */
    void cplt_transfer_callback(void);
    /**
     * @brief hall transfer callback function
     * @note sadly they will need to be handled from the callback
     */
    void half_transfer_callback(void);

};

#endif