#include "wav_player.hpp"

wav_player::wav_player(i2s::i2s_num i2sNum )
{
    i2sx.set_handle(i2sNum);
    //Constructor for now there is nothing special to pass on
    //In the future if we manage to make a FATFS in C++ we pass that on
}
void wav_player::init()
{
  i2sx.start();
}
uint8_t wav_player::file_select(char * filePath)
{

  WAV_HeaderTypeDef wavHeader;
  UINT readBytes = 0;
  //Open WAV file
  if(f_open(&wavFile, filePath, FA_READ) != FR_OK)
  {
    return 0;
  }
  //Read WAV file Header
  f_read(&wavFile, &wavHeader, sizeof(wavHeader), &readBytes);
  //Get audio data size
  fileLength = wavHeader.FileSize;
  //Play the WAV file with frequency specified in header
  samplingFreq = wavHeader.SampleRate;
  return 1;

}


/**
 * @brief WAV File Play
 */
void wav_player::play(void)
{
  isFinished = false;
  //Initialise I2S Audio Sampling settings
  i2sx.init(samplingFreq);
  //Read Audio data from USB Disk
  f_lseek(&wavFile, 0);
  f_read (&wavFile, &audioBuffer[0], AUDIO_BUFFER_SIZE, &playerReadBytes);
  audioRemainSize = fileLength - playerReadBytes;
  //Start playing the WAV

  i2sx.play((uint16_t *)&audioBuffer[0], AUDIO_BUFFER_SIZE);
}

void wav_player::process()
{
  switch(playerControlSM)
  {
  case PLAYER_CONTROL_Idle:
    break;

  case PLAYER_CONTROL_HalfBuffer:
    playerReadBytes = 0;
    playerControlSM = PLAYER_CONTROL_Idle;
    f_read (&wavFile, &audioBuffer[0], AUDIO_BUFFER_SIZE/2, &playerReadBytes);
    if(audioRemainSize > (AUDIO_BUFFER_SIZE / 2))
    {
      audioRemainSize -= playerReadBytes;
    }
    else
    {
      audioRemainSize = 0;
      playerControlSM = PLAYER_CONTROL_EndOfFile;
    }
    break;

  case PLAYER_CONTROL_FullBuffer:
    //audioI2S_resume();
    playerReadBytes = 0;
    playerControlSM = PLAYER_CONTROL_Idle;
    f_read (&wavFile, &audioBuffer[AUDIO_BUFFER_SIZE/2], AUDIO_BUFFER_SIZE/2, &playerReadBytes);
    //myprintf1("File Select Done %s",audioBuffer);

    if(audioRemainSize > (AUDIO_BUFFER_SIZE / 2))
    {
      audioRemainSize -= playerReadBytes;
    }
    else
    {
      audioRemainSize = 0;
      playerControlSM = PLAYER_CONTROL_EndOfFile;
    }
    break;

  case PLAYER_CONTROL_EndOfFile:
    f_close(&wavFile);
    reset();
    isFinished = 1;
    playerControlSM = PLAYER_CONTROL_Idle;
    break;
  }
}

/**
 * @brief WAV stop
 */
void wav_player::stop(void)
{
  i2sx.stop();
  isFinished = true;
}


void wav_player::pause(void)
{
  i2sx.pause();
}

void wav_player::resume(void)
{
  i2sx.resume();
}


bool wav_player::isEndOfFile(void)
{
  return isFinished;
}


void wav_player::reset(void)
{
  audioRemainSize = 0;
  playerReadBytes = 0;
}

/**
 * @brief Half/Full transfer Audio callback for buffer management
 */
void wav_player::half_transfer_callback(void)
{
  playerControlSM = PLAYER_CONTROL_HalfBuffer;
}
void wav_player::cplt_transfer_callback(void)
{
  playerControlSM = PLAYER_CONTROL_FullBuffer;
//  audioI2S_changeBuffer((uint16_t*)&audioBuffer[0], AUDIO_BUFFER_SIZE / 2);
}
