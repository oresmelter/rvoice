#ifndef WAV_HEAD_H
#define WAV_HEAD_H

// WAVE PCM soundfile format
// Структура, описывающая заголовок WAV файла.
typedef struct wheader_file {

    // WAV-формат начинается с RIFF-заголовка:
    // Содержит символы "RIFF" в ASCII кодировке
    // (0x52494646 в big-endian представлении)
    char chunk_id[4];

    // 36 + subchunk2Size, или более точно:
    // 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
    // Это оставшийся размер цепочки, начиная с этой позиции.
    // Иначе говоря, это размер файла - 8, то есть,
    // исключены поля chunkId и chunkSize.
    int chunk_size;

    // Содержит символы "WAVE"
    // (0x57415645 в big-endian представлении)
    char format[4];

    // Формат "WAVE" состоит из двух подцепочек: "fmt " и "data":
    // Подцепочка "fmt " описывает формат звуковых данных:
    // Содержит символы "fmt "
    // (0x666d7420 в big-endian представлении)
    char subchunk1_id[4];

    // 16 для формата PCM.
    // Это оставшийся размер подцепочки, начиная с этой позиции.
    int subchunk1_size;

    // Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
    // Для PCM = 1 (то есть, Линейное квантование).
    // Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    short int audio_format;

    // Количество каналов. Моно = 1, Стерео = 2 и т.д.
    short int num_channels;

    // Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
    int sample_rate;		// sample_rate denotes the sampling rate.

    // sampleRate * numChannels * bitsPerSample/8
    int byte_rate;

    // numChannels * bitsPerSample/8
    // Количество байт для одного сэмпла, включая все каналы.
    short int block_align;

    // Так называемая "глубина" или точность звучания. 8 бит, 16 бит и т.д.
    short int bits_per_sample;

    // Подцепочка "data" содержит аудио-данные и их размер.
    // Содержит символы "data"
    // (0x64617461 в big-endian представлении)
    char subchunk2_id[4];

    // numSamples * numChannels * bitsPerSample/8
    // Количество байт в области данных.
    int subchunk2_size;		// subchunk2_size denotes the number of samples.
    
    // Далее следуют непосредственно Wav данные.
} header;

typedef struct wheader_file *header_p;



static struct {
	int limit;
	int rate;
} const fms_rates[11] = {
	{  6600,  5500 },
	{  8750,  8000 },
	{ 10250,  9600 },
	{ 13200, 11025 },
	{ 17500, 16000 },
	{ 20500, 19200 },
	{ 26500, 22050 },
	{ 35000, 32000 },
	{ 41000, 38400 },
	{ 46000, 44100 },
	{ 48000, 48000 },
	/* anything above -> 48000 */
}; 					// play->sample_rate = fms_rates[i].rate;

#endif
