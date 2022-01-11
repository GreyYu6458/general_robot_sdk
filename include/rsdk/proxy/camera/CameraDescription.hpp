#pragma once


namespace rsdk::camera
{   

    enum class EncodeType
    {
        /* COPIED FROM FFMPEG */
        ID_NONE,
        /* video codecs */
        ID_MPEG1VIDEO,
        ID_MPEG2VIDEO, ///< preferred ID for MPEG-1/2 video decoding
        ID_H261,
        ID_H263,
        ID_RV10,
        ID_RV20,
        ID_MJPEG,
        ID_MJPEGB,
        ID_LJPEG,
        ID_SP5X,
        ID_JPEGLS,
        ID_MPEG4,
        ID_RAWVIDEO,
        ID_MSMPEG4V1,
        ID_MSMPEG4V2,
        ID_MSMPEG4V3,
        ID_WMV1,
        ID_WMV2,
        ID_H263P,
        ID_H263I,
        ID_FLV1,
        ID_SVQ1,
        ID_SVQ3,
        ID_DVVIDEO,
        ID_HUFFYUV,
        ID_CYUV,
        ID_H264,
        ID_INDEO3,
        ID_VP3,
        ID_THEORA,
        ID_ASV1,
        ID_ASV2,
        ID_FFV1,
        ID_4XM,
        ID_VCR1,
        ID_CLJR,
        ID_MDEC,
        ID_ROQ,
        ID_INTERPLAY_VIDEO,
        ID_XAN_WC3,
        ID_XAN_WC4,
        ID_RPZA,
        ID_CINEPAK,
        ID_WS_VQA,
        ID_MSRLE,
        ID_MSVIDEO1,
        ID_IDCIN,
        ID_8BPS,
        ID_SMC,
        ID_FLIC,
        ID_TRUEMOTION1,
        ID_VMDVIDEO,
        ID_MSZH,
        ID_ZLIB,
        ID_QTRLE,
        ID_TSCC,
        ID_ULTI,
        ID_QDRAW,
        ID_VIXL,
        ID_QPEG,
        ID_PNG,
        ID_PPM,
        ID_PBM,
        ID_PGM,
        ID_PGMYUV,
        ID_PAM,
        ID_FFVHUFF,
        ID_RV30,
        ID_RV40,
        ID_VC1,
        ID_WMV3,
        ID_LOCO,
        ID_WNV1,
        ID_AASC,
        ID_INDEO2,
        ID_FRAPS,
        ID_TRUEMOTION2,
        ID_BMP,
        ID_CSCD,
        ID_MMVIDEO,
        ID_ZMBV,
        ID_AVS,
        ID_SMACKVIDEO,
        ID_NUV,
        ID_KMVC,
        ID_FLASHSV,
        ID_CAVS,
        ID_JPEG2000,
        ID_VMNC,
        ID_VP5,
        ID_VP6,
        ID_VP6F,
        ID_TARGA,
        ID_DSICINVIDEO,
        ID_TIERTEXSEQVIDEO,
        ID_TIFF,
        ID_GIF,
        ID_DXA,
        ID_DNXHD,
        ID_THP,
        ID_SGI,
        ID_C93,
        ID_BETHSOFTVID,
        ID_PTX,
        ID_TXD,
        ID_VP6A,
        ID_AMV,
        ID_VB,
        ID_PCX,
        ID_SUNRAST,
        ID_INDEO4,
        ID_INDEO5,
        ID_MIMIC,
        ID_RL2,
        ID_ESCAPE124,
        ID_DIRAC,
        ID_BFI,
        ID_CMV,
        ID_MOTIONPIXELS,
        ID_TGV,
        ID_TGQ,
        ID_TQI,
        ID_AURA,
        ID_AURA2,
        ID_V210X,
        ID_TMV,
        ID_V210,
        ID_DPX,
        ID_MAD,
        ID_FRWU,
        ID_FLASHSV2,
        ID_CDGRAPHICS,
        ID_R210,
        ID_ANM,
        ID_BINKVIDEO,
        ID_IFF_ILBM,
        ID_IFF_BYTERUN1 = ID_IFF_ILBM,
        ID_KGV1,
        ID_YOP,
        ID_VP8,
        ID_PICTOR,
        ID_ANSI,
        ID_A64_MULTI,
        ID_A64_MULTI5,
        ID_R10K,
        ID_MXPEG,
        ID_LAGARITH,
        ID_PRORES,
        ID_JV,
        ID_DFA,
        ID_WMV3IMAGE,
        ID_VC1IMAGE,
        ID_UTVIDEO,
        ID_BMV_VIDEO,
        ID_VBLE,
        ID_DXTORY,
        ID_V410,
        ID_XWD,
        ID_CDXL,
        ID_XBM,
        ID_ZEROCODEC,
        ID_MSS1,
        ID_MSA1,
        ID_TSCC2,
        ID_MTS2,
        ID_CLLC,
        ID_MSS2,
        ID_VP9,
        ID_AIC,
        ID_ESCAPE130,
        ID_G2M,
        ID_WEBP,
        ID_HNM4_VIDEO,
        ID_HEVC,
        ID_H265 = ID_HEVC,
        ID_FIC,
        ID_ALIAS_PIX,
        ID_BRENDER_PIX,
        ID_PAF_VIDEO,
        ID_EXR,
        ID_VP7,
        ID_SANM,
        ID_SGIRLE,
        ID_MVC1,
        ID_MVC2,
        ID_HQX,
        ID_TDSC,
        ID_HQ_HQA,
        ID_HAP,
        ID_DDS,
        ID_DXV,
        ID_SCREENPRESSO,
        ID_RSCC,
        ID_AVS2,

        ID_Y41P,
        ID_AVRP,
        ID_012V,
        ID_AVUI,
        ID_AYUV,
        ID_TARGA_Y216,
        ID_V308,
        ID_V408,
        ID_YUV4,
        ID_AVRN,
        ID_CPIA,
        ID_XFACE,
        ID_SNOW,
        ID_SMVJPEG,
        ID_APNG,
        ID_DAALA,
        ID_CFHD,
        ID_TRUEMOTION2RT,
        ID_M101,
        ID_MAGICYUV,
        ID_SHEERVIDEO,
        ID_YLC,
        ID_PSD,
        ID_PIXLET,
        ID_SPEEDHQ,
        ID_FMVC,
        ID_SCPR,
        ID_CLEARVIDEO,
        ID_XPM,
        ID_AV1,
        ID_BITPACKED,
        ID_MSCC,
        ID_SRGC,
        ID_SVG,
        ID_GDV,
        ID_FITS,
        ID_IMM4,
        ID_PROSUMER,
        ID_MWSC,
        ID_WCMV,
        ID_RASC,
        ID_HYMT,
        ID_ARBC,
        ID_AGM,
        ID_LSCR,
        ID_VP4
    };

    enum class PixFmt
    {
        /* COPIED FROM FFMPEG */
        NONE = -1,
        YUV420P,   ///< planar YUV 4:2:0, 12bpp, (1 Cr & Cb sample per 2x2 Y samples)
        YUYV422,   ///< packed YUV 4:2:2, 16bpp, Y0 Cb Y1 Cr
        RGB24,     ///< packed RGB 8:8:8, 24bpp, RGBRGB...
        BGR24,     ///< packed RGB 8:8:8, 24bpp, BGRBGR...
        YUV422P,   ///< planar YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
        YUV444P,   ///< planar YUV 4:4:4, 24bpp, (1 Cr & Cb sample per 1x1 Y samples)
        YUV410P,   ///< planar YUV 4:1:0,  9bpp, (1 Cr & Cb sample per 4x4 Y samples)
        YUV411P,   ///< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples)
        GRAY8,     ///<        Y        ,  8bpp
        MONOWHITE, ///<        Y        ,  1bpp, 0 is white, 1 is black, in each byte pixels are ordered from the msb to the lsb
        MONOBLACK, ///<        Y        ,  1bpp, 0 is black, 1 is white, in each byte pixels are ordered from the msb to the lsb
        PAL8,      ///< 8 bits with     RGB32 palette
        YUVJ420P,  ///< planar YUV 4:2:0, 12bpp, full scale (JPEG), deprecated in favor of  YUV420P and setting color_range
        YUVJ422P,  ///< planar YUV 4:2:2, 16bpp, full scale (JPEG), deprecated in favor of  YUV422P and setting color_range
        YUVJ444P,  ///< planar YUV 4:4:4, 24bpp, full scale (JPEG), deprecated in favor of  YUV444P and setting color_range
        UYVY422,   ///< packed YUV 4:2:2, 16bpp, Cb Y0 Cr Y1
        UYYVYY411, ///< packed YUV 4:1:1, 12bpp, Cb Y0 Y1 Cr Y2 Y3
        BGR8,      ///< packed RGB 3:3:2,  8bpp, (msb)2B 3G 3R(lsb)
        BGR4,      ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1B 2G 1R(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits
        BGR4_BYTE, ///< packed RGB 1:2:1,  8bpp, (msb)1B 2G 1R(lsb)
        RGB8,      ///< packed RGB 3:3:2,  8bpp, (msb)2R 3G 3B(lsb)
        RGB4,      ///< packed RGB 1:2:1 bitstream,  4bpp, (msb)1R 2G 1B(lsb), a byte contains two pixels, the first pixel in the byte is the one composed by the 4 msb bits
        RGB4_BYTE, ///< packed RGB 1:2:1,  8bpp, (msb)1R 2G 1B(lsb)
        NV12,      ///< planar YUV 4:2:0, 12bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)
        NV21,      ///< as above, but U and V bytes are swapped

        ARGB,      ///< packed ARGB 8:8:8:8, 32bpp, ARGBARGB...
        RGBA,      ///< packed RGBA 8:8:8:8, 32bpp, RGBARGBA...
        ABGR,      ///< packed ABGR 8:8:8:8, 32bpp, ABGRABGR...
        BGRA,      ///< packed BGRA 8:8:8:8, 32bpp, BGRABGRA...

        GRAY16BE,  ///<        Y        , 16bpp, big-endian
        GRAY16LE,  ///<        Y        , 16bpp, little-endian
        YUV440P,   ///< planar YUV 4:4:0 (1 Cr & Cb sample per 1x2 Y samples)
        YUVJ440P,  ///< planar YUV 4:4:0 full scale (JPEG), deprecated in favor of  YUV440P and setting color_range
        YUVA420P,  ///< planar YUV 4:2:0, 20bpp, (1 Cr & Cb sample per 2x2 Y & A samples)
        RGB48BE,   ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as big-endian
        RGB48LE,   ///< packed RGB 16:16:16, 48bpp, 16R, 16G, 16B, the 2-byte value for each R/G/B component is stored as little-endian

        RGB565BE,  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), big-endian
        RGB565LE,  ///< packed RGB 5:6:5, 16bpp, (msb)   5R 6G 5B(lsb), little-endian
        RGB555BE,  ///< packed RGB 5:5:5, 16bpp, (msb)1X 5R 5G 5B(lsb), big-endian   , X=unused/undefined
        RGB555LE,  ///< packed RGB 5:5:5, 16bpp, (msb)1X 5R 5G 5B(lsb), little-endian, X=unused/undefined

        BGR565BE,  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), big-endian
        BGR565LE,  ///< packed BGR 5:6:5, 16bpp, (msb)   5B 6G 5R(lsb), little-endian
        BGR555BE,  ///< packed BGR 5:5:5, 16bpp, (msb)1X 5B 5G 5R(lsb), big-endian   , X=unused/undefined
        BGR555LE,  ///< packed BGR 5:5:5, 16bpp, (msb)1X 5B 5G 5R(lsb), little-endian, X=unused/undefined

        YUV420P16LE,  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
        YUV420P16BE,  ///< planar YUV 4:2:0, 24bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
        YUV422P16LE,  ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
        YUV422P16BE,  ///< planar YUV 4:2:2, 32bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
        YUV444P16LE,  ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
        YUV444P16BE,  ///< planar YUV 4:4:4, 48bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
        DXVA2_VLD,    ///< HW decoding through DXVA2, Picture.data[3] contains a LPDIRECT3DSURFACE9 pointer

        RGB444LE,  ///< packed RGB 4:4:4, 16bpp, (msb)4X 4R 4G 4B(lsb), little-endian, X=unused/undefined
        RGB444BE,  ///< packed RGB 4:4:4, 16bpp, (msb)4X 4R 4G 4B(lsb), big-endian,    X=unused/undefined
        BGR444LE,  ///< packed BGR 4:4:4, 16bpp, (msb)4X 4B 4G 4R(lsb), little-endian, X=unused/undefined
        BGR444BE,  ///< packed BGR 4:4:4, 16bpp, (msb)4X 4B 4G 4R(lsb), big-endian,    X=unused/undefined
        YA8,       ///< 8 bits gray, 8 bits alpha

        Y400A =     YA8, ///< alias for     YA8
        GRAY8A=     YA8, ///< alias for     YA8

        BGR48BE,   ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as big-endian
        BGR48LE,   ///< packed RGB 16:16:16, 48bpp, 16B, 16G, 16R, the 2-byte value for each R/G/B component is stored as little-endian

        /**
         * The following 12 formats have the disadvantage of needing 1 format for each bit depth.
         * Notice that each 9/10 bits sample is stored in 16 bits with extra padding.
         * If you want to support multiple bit depths, then using   YUV420P16* with the bpp stored separately is better.
         */
        YUV420P9BE, ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
        YUV420P9LE, ///< planar YUV 4:2:0, 13.5bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
        YUV420P10BE,///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
        YUV420P10LE,///< planar YUV 4:2:0, 15bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
        YUV422P10BE,///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
        YUV422P10LE,///< planar YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
        YUV444P9BE, ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
        YUV444P9LE, ///< planar YUV 4:4:4, 27bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
        YUV444P10BE,///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
        YUV444P10LE,///< planar YUV 4:4:4, 30bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
        YUV422P9BE, ///< planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
        YUV422P9LE, ///< planar YUV 4:2:2, 18bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
        GBRP,      ///< planar GBR 4:4:4 24bpp
        GBR24P =    GBRP, // alias for #   GBRP
        GBRP9BE,   ///< planar GBR 4:4:4 27bpp, big-endian
        GBRP9LE,   ///< planar GBR 4:4:4 27bpp, little-endian
        GBRP10BE,  ///< planar GBR 4:4:4 30bpp, big-endian
        GBRP10LE,  ///< planar GBR 4:4:4 30bpp, little-endian
        GBRP16BE,  ///< planar GBR 4:4:4 48bpp, big-endian
        GBRP16LE,  ///< planar GBR 4:4:4 48bpp, little-endian
        YUVA422P,  ///< planar YUV 4:2:2 24bpp, (1 Cr & Cb sample per 2x1 Y & A samples)
        YUVA444P,  ///< planar YUV 4:4:4 32bpp, (1 Cr & Cb sample per 1x1 Y & A samples)
        YUVA420P9BE,  ///< planar YUV 4:2:0 22.5bpp, (1 Cr & Cb sample per 2x2 Y & A samples), big-endian
        YUVA420P9LE,  ///< planar YUV 4:2:0 22.5bpp, (1 Cr & Cb sample per 2x2 Y & A samples), little-endian
        YUVA422P9BE,  ///< planar YUV 4:2:2 27bpp, (1 Cr & Cb sample per 2x1 Y & A samples), big-endian
        YUVA422P9LE,  ///< planar YUV 4:2:2 27bpp, (1 Cr & Cb sample per 2x1 Y & A samples), little-endian
        YUVA444P9BE,  ///< planar YUV 4:4:4 36bpp, (1 Cr & Cb sample per 1x1 Y & A samples), big-endian
        YUVA444P9LE,  ///< planar YUV 4:4:4 36bpp, (1 Cr & Cb sample per 1x1 Y & A samples), little-endian
        YUVA420P10BE, ///< planar YUV 4:2:0 25bpp, (1 Cr & Cb sample per 2x2 Y & A samples, big-endian)
        YUVA420P10LE, ///< planar YUV 4:2:0 25bpp, (1 Cr & Cb sample per 2x2 Y & A samples, little-endian)
        YUVA422P10BE, ///< planar YUV 4:2:2 30bpp, (1 Cr & Cb sample per 2x1 Y & A samples, big-endian)
        YUVA422P10LE, ///< planar YUV 4:2:2 30bpp, (1 Cr & Cb sample per 2x1 Y & A samples, little-endian)
        YUVA444P10BE, ///< planar YUV 4:4:4 40bpp, (1 Cr & Cb sample per 1x1 Y & A samples, big-endian)
        YUVA444P10LE, ///< planar YUV 4:4:4 40bpp, (1 Cr & Cb sample per 1x1 Y & A samples, little-endian)
        YUVA420P16BE, ///< planar YUV 4:2:0 40bpp, (1 Cr & Cb sample per 2x2 Y & A samples, big-endian)
        YUVA420P16LE, ///< planar YUV 4:2:0 40bpp, (1 Cr & Cb sample per 2x2 Y & A samples, little-endian)
        YUVA422P16BE, ///< planar YUV 4:2:2 48bpp, (1 Cr & Cb sample per 2x1 Y & A samples, big-endian)
        YUVA422P16LE, ///< planar YUV 4:2:2 48bpp, (1 Cr & Cb sample per 2x1 Y & A samples, little-endian)
        YUVA444P16BE, ///< planar YUV 4:4:4 64bpp, (1 Cr & Cb sample per 1x1 Y & A samples, big-endian)
        YUVA444P16LE, ///< planar YUV 4:4:4 64bpp, (1 Cr & Cb sample per 1x1 Y & A samples, little-endian)

        VDPAU,     ///< HW acceleration through VDPAU, Picture.data[3] contains a VdpVideoSurface

        XYZ12LE,      ///< packed XYZ 4:4:4, 36 bpp, (msb) 12X, 12Y, 12Z (lsb), the 2-byte value for each X/Y/Z is stored as little-endian, the 4 lower bits are set to 0
        XYZ12BE,      ///< packed XYZ 4:4:4, 36 bpp, (msb) 12X, 12Y, 12Z (lsb), the 2-byte value for each X/Y/Z is stored as big-endian, the 4 lower bits are set to 0
        NV16,         ///< interleaved chroma YUV 4:2:2, 16bpp, (1 Cr & Cb sample per 2x1 Y samples)
        NV20LE,       ///< interleaved chroma YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
        NV20BE,       ///< interleaved chroma YUV 4:2:2, 20bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian

        RGBA64BE,     ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
        RGBA64LE,     ///< packed RGBA 16:16:16:16, 64bpp, 16R, 16G, 16B, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian
        BGRA64BE,     ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as big-endian
        BGRA64LE,     ///< packed RGBA 16:16:16:16, 64bpp, 16B, 16G, 16R, 16A, the 2-byte value for each R/G/B/A component is stored as little-endian

        YVYU422,   ///< packed YUV 4:2:2, 16bpp, Y0 Cr Y1 Cb

        YA16BE,       ///< 16 bits gray, 16 bits alpha (big-endian)
        YA16LE,       ///< 16 bits gray, 16 bits alpha (little-endian)

        GBRAP,        ///< planar GBRA 4:4:4:4 32bpp
        GBRAP16BE,    ///< planar GBRA 4:4:4:4 64bpp, big-endian
        GBRAP16LE,    ///< planar GBRA 4:4:4:4 64bpp, little-endian
    /**
     *  HW acceleration through QSV, data[3] contains a pointer to the
     *  mfxFrameSurface1 structure.
     */
        QSV,
    /**
     * HW acceleration though MMAL, data[3] contains a pointer to the
     * MMAL_BUFFER_HEADER_T structure.
     */
        MMAL,

        D3D11VA_VLD,  ///< HW decoding through Direct3D11 via old API, Picture.data[3] contains a ID3D11VideoDecoderOutputView pointer

        /**
         * HW acceleration through CUDA. data[i] contain CUdeviceptr pointers
         * exactly as for system memory frames.
         */
        CUDA,

        _0RGB,        ///< packed RGB 8:8:8, 32bpp, XRGBXRGB...   X=unused/undefined
        RGB0,        ///< packed RGB 8:8:8, 32bpp, RGBXRGBX...   X=unused/undefined
        _0BGR,        ///< packed BGR 8:8:8, 32bpp, XBGRXBGR...   X=unused/undefined
        BGR0,        ///< packed BGR 8:8:8, 32bpp, BGRXBGRX...   X=unused/undefined

        YUV420P12BE, ///< planar YUV 4:2:0,18bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
        YUV420P12LE, ///< planar YUV 4:2:0,18bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
        YUV420P14BE, ///< planar YUV 4:2:0,21bpp, (1 Cr & Cb sample per 2x2 Y samples), big-endian
        YUV420P14LE, ///< planar YUV 4:2:0,21bpp, (1 Cr & Cb sample per 2x2 Y samples), little-endian
        YUV422P12BE, ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
        YUV422P12LE, ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
        YUV422P14BE, ///< planar YUV 4:2:2,28bpp, (1 Cr & Cb sample per 2x1 Y samples), big-endian
        YUV422P14LE, ///< planar YUV 4:2:2,28bpp, (1 Cr & Cb sample per 2x1 Y samples), little-endian
        YUV444P12BE, ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
        YUV444P12LE, ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
        YUV444P14BE, ///< planar YUV 4:4:4,42bpp, (1 Cr & Cb sample per 1x1 Y samples), big-endian
        YUV444P14LE, ///< planar YUV 4:4:4,42bpp, (1 Cr & Cb sample per 1x1 Y samples), little-endian
        GBRP12BE,    ///< planar GBR 4:4:4 36bpp, big-endian
        GBRP12LE,    ///< planar GBR 4:4:4 36bpp, little-endian
        GBRP14BE,    ///< planar GBR 4:4:4 42bpp, big-endian
        GBRP14LE,    ///< planar GBR 4:4:4 42bpp, little-endian
        YUVJ411P,    ///< planar YUV 4:1:1, 12bpp, (1 Cr & Cb sample per 4x1 Y samples) full scale (JPEG), deprecated in favor of   YUV411P and setting color_range

        BAYER_BGGR8,    ///< bayer, BGBG..(odd line), GRGR..(even line), 8-bit samples */
        BAYER_RGGB8,    ///< bayer, RGRG..(odd line), GBGB..(even line), 8-bit samples */
        BAYER_GBRG8,    ///< bayer, GBGB..(odd line), RGRG..(even line), 8-bit samples */
        BAYER_GRBG8,    ///< bayer, GRGR..(odd line), BGBG..(even line), 8-bit samples */
        BAYER_BGGR16LE, ///< bayer, BGBG..(odd line), GRGR..(even line), 16-bit samples, little-endian */
        BAYER_BGGR16BE, ///< bayer, BGBG..(odd line), GRGR..(even line), 16-bit samples, big-endian */
        BAYER_RGGB16LE, ///< bayer, RGRG..(odd line), GBGB..(even line), 16-bit samples, little-endian */
        BAYER_RGGB16BE, ///< bayer, RGRG..(odd line), GBGB..(even line), 16-bit samples, big-endian */
        BAYER_GBRG16LE, ///< bayer, GBGB..(odd line), RGRG..(even line), 16-bit samples, little-endian */
        BAYER_GBRG16BE, ///< bayer, GBGB..(odd line), RGRG..(even line), 16-bit samples, big-endian */
        BAYER_GRBG16LE, ///< bayer, GRGR..(odd line), BGBG..(even line), 16-bit samples, little-endian */
        BAYER_GRBG16BE, ///< bayer, GRGR..(odd line), BGBG..(even line), 16-bit samples, big-endian */

        XVMC,///< XVideo Motion Acceleration via common packet passing

        YUV440P10LE, ///< planar YUV 4:4:0,20bpp, (1 Cr & Cb sample per 1x2 Y samples), little-endian
        YUV440P10BE, ///< planar YUV 4:4:0,20bpp, (1 Cr & Cb sample per 1x2 Y samples), big-endian
        YUV440P12LE, ///< planar YUV 4:4:0,24bpp, (1 Cr & Cb sample per 1x2 Y samples), little-endian
        YUV440P12BE, ///< planar YUV 4:4:0,24bpp, (1 Cr & Cb sample per 1x2 Y samples), big-endian
        AYUV64LE,    ///< packed AYUV 4:4:4,64bpp (1 Cr & Cb sample per 1x1 Y & A samples), little-endian
        AYUV64BE,    ///< packed AYUV 4:4:4,64bpp (1 Cr & Cb sample per 1x1 Y & A samples), big-endian

        VIDEOTOOLBOX, ///< hardware decoding through Videotoolbox

        P010LE, ///< like NV12, with 10bpp per component, data in the high bits, zeros in the low bits, little-endian
        P010BE, ///< like NV12, with 10bpp per component, data in the high bits, zeros in the low bits, big-endian

        GBRAP12BE,  ///< planar GBR 4:4:4:4 48bpp, big-endian
        GBRAP12LE,  ///< planar GBR 4:4:4:4 48bpp, little-endian

        GBRAP10BE,  ///< planar GBR 4:4:4:4 40bpp, big-endian
        GBRAP10LE,  ///< planar GBR 4:4:4:4 40bpp, little-endian

        MEDIACODEC, ///< hardware decoding through MediaCodec

        GRAY12BE,   ///<        Y        , 12bpp, big-endian
        GRAY12LE,   ///<        Y        , 12bpp, little-endian
        GRAY10BE,   ///<        Y        , 10bpp, big-endian
        GRAY10LE,   ///<        Y        , 10bpp, little-endian

        P016LE, ///< like NV12, with 16bpp per component, little-endian
        P016BE, ///< like NV12, with 16bpp per component, big-endian

    /**
     * Hardware surfaces for Direct3D11.
     *
     * This is preferred over the legacy    D3D11VA_VLD. The new D3D11
     * hwaccel API and filtering support    D3D11 only.
     *
     * data[0] contains a ID3D11Texture2D pointer, and data[1] contains the
     * texture array index of the frame as intptr_t if the ID3D11Texture2D is
     * an array texture (or always 0 if it's a normal texture).
     */
        D3D11,

        GRAY9BE,   ///<        Y        , 9bpp, big-endian
        GRAY9LE,   ///<        Y        , 9bpp, little-endian

        GBRPF32BE,  ///< IEEE-754 single precision planar GBR 4:4:4,     96bpp, big-endian
        GBRPF32LE,  ///< IEEE-754 single precision planar GBR 4:4:4,     96bpp, little-endian
        GBRAPF32BE, ///< IEEE-754 single precision planar GBRA 4:4:4:4, 128bpp, big-endian
        GBRAPF32LE, ///< IEEE-754 single precision planar GBRA 4:4:4:4, 128bpp, little-endian

    /**
     * DRM-managed buffers exposed through PRIME buffer sharing.
     *
     * data[0] points to an AVDRMFrameDescriptor.
     */
        DRM_PRIME,
    /**
     * Hardware surfaces for OpenCL.
     *
     * data[i] contain 2D image objects (typed in C as cl_mem, used
     * in OpenCL as image2d_t) for each plane of the surface.
     */
        OPENCL,

        GRAY14BE,   ///<        Y        , 14bpp, big-endian
        GRAY14LE,   ///<        Y        , 14bpp, little-endian

        GRAYF32BE,  ///< IEEE-754 single precision Y, 32bpp, big-endian
        GRAYF32LE,  ///< IEEE-754 single precision Y, 32bpp, little-endian

        YUVA422P12BE, ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), 12b alpha, big-endian
        YUVA422P12LE, ///< planar YUV 4:2:2,24bpp, (1 Cr & Cb sample per 2x1 Y samples), 12b alpha, little-endian
        YUVA444P12BE, ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), 12b alpha, big-endian
        YUVA444P12LE, ///< planar YUV 4:4:4,36bpp, (1 Cr & Cb sample per 1x1 Y samples), 12b alpha, little-endian

        NV24,      ///< planar YUV 4:4:4, 24bpp, 1 plane for Y and 1 plane for the UV components, which are interleaved (first byte U and the following byte V)
        NV42,      ///< as above, but U and V bytes are swapped

        NB         ///< number of pixel formats, DO NOT USE THIS if you want to link with shared libav* because the number of formats might differ between versions
    };
}