#pragma once
#include <xaudio2.h>
#include <fstream>
#include <wrl.h>

using namespace Microsoft::WRL;

#pragma comment(lib,"xaudio2.lib")

struct ChunkHeader {
	char id[4];
	int32_t size;
};

struct RiffHeader {
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk {
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

struct SoundData {
	//波系フォーマット
	WAVEFORMATEX wfex;
	//バッファの先頭アドレス
	BYTE* pBuffer;
	//バッファのサイズ
	unsigned int bufferSize;
};

class Sound
{
public:

	void Initialize();

	void Update();

	//音声データのアンロード(再生した後使用)
	void UnLoad(SoundData* soundData);

	//音声再生
	void PlayWave(const SoundData& soundData, bool isLoop, float volume);

	//音声を止める
	void StopSound();

	//XAudio2の解放
	void Finalize();

	SoundData LoadWave(const char* filename);

	ComPtr<IXAudio2> GetXAudio2() { return xAudio2; }

private:

	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;

	IXAudio2SourceVoice* pSourceVoice = nullptr;

	HRESULT result;

};