#include <faac.h>
#include <stdio.h>

typedef unsigned long   ULONG;
typedef unsigned int    UINT;
typedef unsigned char   BYTE;
typedef char            _TCHAR;

int main(int argc, _TCHAR* argv[])
{
    ULONG nSampleRate = 48000;  // ������
    UINT nChannels = 2;         // ������
    UINT nPCMBitSize = 16;      // ������λ��
    ULONG nInputSamples = 0;
    ULONG nMaxOutputBytes = 0;

    int nRet;
    faacEncHandle hEncoder;
    faacEncConfigurationPtr pConfiguration; 

    int nBytesRead;
    int nPCMBufferSize;
    BYTE* pbPCMBuffer;
    BYTE* pbAACBuffer;

    FILE* fpIn; // PCM file for input
    FILE* fpOut; // AAC file for output

    
    if(argc < 3)
    {
        printf("eg: ./pcm_aac_test input.pcm output.aac\n");
        return 0;
    }  
    printf("pcm to aac : %s to %s",argv[1],argv[2]);
    fpIn = fopen(argv[1], "rb");
    fpOut = fopen(argv[2], "wb");

    // (1) Open FAAC engine
    hEncoder = faacEncOpen(nSampleRate, nChannels, &nInputSamples, &nMaxOutputBytes);
    if(hEncoder == NULL)
    {
        printf("[ERROR] Failed to call faacEncOpen()\n");
        return -1;
    }

    nPCMBufferSize = nInputSamples * nPCMBitSize / 8;
    pbPCMBuffer = new BYTE [nPCMBufferSize];
    pbAACBuffer = new BYTE [nMaxOutputBytes];

    // (2.1) Get current encoding configuration
    pConfiguration = faacEncGetCurrentConfiguration(hEncoder);
    pConfiguration->inputFormat = FAAC_INPUT_16BIT;

    // (2.2) Set encoding configuration
    nRet = faacEncSetConfiguration(hEncoder, pConfiguration);

    for(int i = 0; 1; i++)
    {
        // �����ʵ���ֽ�������󲻻ᳬ��nPCMBufferSize��һ��ֻ�ж����ļ�βʱ�Ų������ֵ
        nBytesRead = fread(pbPCMBuffer, 1, nPCMBufferSize, fpIn);

        printf("read :  %d:%d\n", nBytesRead, nPCMBufferSize);
        // ��������������ʵ�ʶ����ֽ������㣬һ��ֻ�ж����ļ�βʱ�Ų���nPCMBufferSize/(nPCMBitSize/8);
        nInputSamples = nBytesRead / (nPCMBitSize / 8);

        // (3) Encode
        nRet = faacEncEncode(
        hEncoder, (int*) pbPCMBuffer, nInputSamples, pbAACBuffer, nMaxOutputBytes);

        fwrite(pbAACBuffer, 1, nRet, fpOut);

        printf("%d: faacEncEncode returns %d\n", i, nRet);

        if(nBytesRead <= 0)
        {
            break;
        }
    }

    /*
    while(1)
    {
        // (3) Flushing
        nRet = faacEncEncode(
        hEncoder, (int*) pbPCMBuffer, 0, pbAACBuffer, nMaxOutputBytes);

        if(nRet <= 0)
        {
            break;
        }
    }
    */

    // (4) Close FAAC engine
    nRet = faacEncClose(hEncoder);

    delete[] pbPCMBuffer;
    delete[] pbAACBuffer;
    fclose(fpIn);
    fclose(fpOut);

    //getchar();

    return 0;
}
