#ifndef AL_AL_H
#define AL_AL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef AL_API
#if defined(AL_LIBTYPE_STATIC)
#define AL_API
#elif defined(_WIN32)
#define AL_API __declspec(dllimport)
#else
#define AL_API extern
#endif
#endif

#ifdef _WIN32
#define AL_APIENTRY __cdecl
#else
#define AL_APIENTRY
#endif

/* Deprecated macros. */
#define OPENAL
#define ALAPI AL_API
#define ALAPIENTRY AL_APIENTRY
#define AL_INVALID (-1)
#define AL_ILLEGAL_ENUM AL_INVALID_ENUM
#define AL_ILLEGAL_COMMAND AL_INVALID_OPERATION

/* Supported AL versions. */
#define AL_VERSION_1_0
#define AL_VERSION_1_1

/** 8-bit boolean */
typedef char ALboolean;

/** character */
typedef char ALchar;

/** signed 8-bit integer */
typedef signed char ALbyte;

/** unsigned 8-bit integer */
typedef unsigned char ALubyte;

/** signed 16-bit integer */
typedef short ALshort;

/** unsigned 16-bit integer */
typedef unsigned short ALushort;

/** signed 32-bit integer */
typedef int ALint;

/** unsigned 32-bit integer */
typedef unsigned int ALuint;

/** non-negative 32-bit integer size */
typedef int ALsizei;

/** 32-bit enumeration value */
typedef int ALenum;

/** 32-bit IEEE-754 floating-point */
typedef float ALfloat;

/** 64-bit IEEE-754 floating-point */
typedef double ALdouble;

/** void type (opaque pointers only) */
typedef void ALvoid;

/* Enumeration values begin at column 50. Do not use tabs. */

/** No distance model or no buffer */
#define AL_NONE 0

/** Boolean False. */
#define AL_FALSE 0

/** Boolean True. */
#define AL_TRUE 1

/**
 * Relative source.
 * Type:    ALboolean
 * Range:   [AL_FALSE, AL_TRUE]
 * Default: AL_FALSE
 *
 * Specifies if the source uses relative coordinates.
 */
#define AL_SOURCE_RELATIVE 0x202

/**
 * Inner cone angle, in degrees.
 * Type:    ALint, ALfloat
 * Range:   [0 - 360]
 * Default: 360
 *
 * The angle covered by the inner cone, the area within which the source will
 * not be attenuated by direction.
 */
#define AL_CONE_INNER_ANGLE 0x1001

/**
 * Outer cone angle, in degrees.
 * Range:   [0 - 360]
 * Default: 360
 *
 * The angle covered by the outer cone, the area outside of which the source
 * will be fully attenuated by direction.
 */
#define AL_CONE_OUTER_ANGLE 0x1002

/**
 * Source pitch.
 * Type:    ALfloat
 * Range:   [0.5 - 2.0]
 * Default: 1.0
 *
 * A multiplier for the sample rate of the source's buffer.
 */
#define AL_PITCH 0x1003

/**
 * Source or listener position.
 * Type:    ALfloat[3], ALint[3]
 * Default: {0, 0, 0}
 *
 * The source or listener location in three dimensional space.
 *
 * OpenAL uses a right handed coordinate system, like OpenGL, where with a
 * default view, X points right (thumb), Y points up (index finger), and Z
 * points towards the viewer/camera (middle finger).
 *
 * To change from or to a left handed coordinate system, negate the Z
 * component.
 */
#define AL_POSITION 0x1004

/**
 * Source direction.
 * Type:    ALfloat[3], ALint[3]
 * Default: {0, 0, 0}
 *
 * Specifies the current direction in local space. A zero-length vector
 * specifies an omni-directional source (cone is ignored).
 *
 * To change from or to a left handed coordinate system, negate the Z
 * component.
 */
#define AL_DIRECTION 0x1005

/**
 * Source or listener velocity.
 * Type:    ALfloat[3], ALint[3]
 * Default: {0, 0, 0}
 *
 * Specifies the current velocity, relative to the position.
 *
 * To change from or to a left handed coordinate system, negate the Z
 * component.
 */
#define AL_VELOCITY 0x1006

/**
 * Source looping.
 * Type:    ALboolean
 * Range:   [AL_FALSE, AL_TRUE]
 * Default: AL_FALSE
 *
 * Specifies whether source playback loops.
 */
#define AL_LOOPING 0x1007

/**
 * Source buffer.
 * Type:    ALuint
 * Range:   any valid Buffer ID
 * Default: AL_NONE
 *
 * Specifies the buffer to provide sound samples for a source.
 */
#define AL_BUFFER 0x1009

/**
 * Source or listener gain.
 * Type:  ALfloat
 * Range: [0.0 - ]
 *
 * For sources, an initial linear gain value (before attenuation is applied).
 * For the listener, an output linear gain adjustment.
 *
 * A value of 1.0 means unattenuated. Each division by 2 equals an attenuation
 * of about -6dB. Each multiplication by 2 equals an amplification of about
 * +6dB.
 */
#define AL_GAIN 0x100A

/**
 * Minimum source gain.
 * Type:  ALfloat
 * Range: [0.0 - 1.0]
 *
 * The minimum gain allowed for a source, after distance and cone attenuation
 * are applied (if applicable).
 */
#define AL_MIN_GAIN 0x100D

/**
 * Maximum source gain.
 * Type:  ALfloat
 * Range: [0.0 - 1.0]
 *
 * The maximum gain allowed for a source, after distance and cone attenuation
 * are applied (if applicable).
 */
#define AL_MAX_GAIN 0x100E

/**
 * Listener orientation.
 * Type:    ALfloat[6]
 * Default: {0.0, 0.0, -1.0, 0.0, 1.0, 0.0}
 *
 * Effectively two three dimensional vectors. The first vector is the front (or
 * "at") and the second is the top (or "up"). Both vectors are relative to the
 * listener position.
 *
 * To change from or to a left handed coordinate system, negate the Z
 * component of both vectors.
 */
#define AL_ORIENTATION 0x100F

/**
 * Source state (query only).
 * Type:  ALenum
 * Range: [AL_INITIAL, AL_PLAYING, AL_PAUSED, AL_STOPPED]
 */
#define AL_SOURCE_STATE 0x1010

/* Source state values. */
#define AL_INITIAL 0x1011
#define AL_PLAYING 0x1012
#define AL_PAUSED 0x1013
#define AL_STOPPED 0x1014

/**
 * Source Buffer Queue size (query only).
 * Type: ALint
 *
 * The number of buffers queued using alSourceQueueBuffers, minus the buffers
 * removed with alSourceUnqueueBuffers.
 */
#define AL_BUFFERS_QUEUED 0x1015

/**
 * Source Buffer Queue processed count (query only).
 * Type: ALint
 *
 * The number of queued buffers that have been fully processed, and can be
 * removed with alSourceUnqueueBuffers.
 *
 * Looping sources will never fully process buffers because they will be set to
 * play again for when the source loops.
 */
#define AL_BUFFERS_PROCESSED 0x1016

/**
 * Source reference distance.
 * Type:    ALfloat
 * Range:   [0.0 - ]
 * Default: 1.0
 *
 * The distance in units that no distance attenuation occurs.
 *
 * At 0.0, no distance attenuation occurs with non-linear attenuation models.
 */
#define AL_REFERENCE_DISTANCE 0x1020

/**
 * Source rolloff factor.
 * Type:    ALfloat
 * Range:   [0.0 - ]
 * Default: 1.0
 *
 * Multiplier to exaggerate or diminish distance attenuation.
 *
 * At 0.0, no distance attenuation ever occurs.
 */
#define AL_ROLLOFF_FACTOR 0x1021

/**
 * Outer cone gain.
 * Type:    ALfloat
 * Range:   [0.0 - 1.0]
 * Default: 0.0
 *
 * The gain attenuation applied when the listener is outside of the source's
 * outer cone angle.
 */
#define AL_CONE_OUTER_GAIN 0x1022

/**
 * Source maximum distance.
 * Type:    ALfloat
 * Range:   [0.0 - ]
 * Default: FLT_MAX
 *
 * The distance above which the source is not attenuated any further with a
 * clamped distance model, or where attenuation reaches 0.0 gain for linear
 * distance models with a default rolloff factor.
 */
#define AL_MAX_DISTANCE 0x1023

/** Source buffer offset, in seconds */
#define AL_SEC_OFFSET 0x1024
/** Source buffer offset, in sample frames */
#define AL_SAMPLE_OFFSET 0x1025
/** Source buffer offset, in bytes */
#define AL_BYTE_OFFSET 0x1026

/**
 * Source type (query only).
 * Type:  ALenum
 * Range: [AL_STATIC, AL_STREAMING, AL_UNDETERMINED]
 *
 * A Source is Static if a Buffer has been attached using AL_BUFFER.
 *
 * A Source is Streaming if one or more Buffers have been attached using
 * alSourceQueueBuffers.
 *
 * A Source is Undetermined when it has the NULL buffer attached using
 * AL_BUFFER.
 */
#define AL_SOURCE_TYPE 0x1027

/* Source type values. */
#define AL_STATIC 0x1028
#define AL_STREAMING 0x1029
#define AL_UNDETERMINED 0x1030

/** Unsigned 8-bit mono buffer format. */
#define AL_FORMAT_MONO8 0x1100
/** Signed 16-bit mono buffer format. */
#define AL_FORMAT_MONO16 0x1101
/** Unsigned 8-bit stereo buffer format. */
#define AL_FORMAT_STEREO8 0x1102
/** Signed 16-bit stereo buffer format. */
#define AL_FORMAT_STEREO16 0x1103

/** Buffer frequency/sample rate (query only). */
#define AL_FREQUENCY 0x2001
/** Buffer bits per sample (query only). */
#define AL_BITS 0x2002
/** Buffer channel count (query only). */
#define AL_CHANNELS 0x2003
/** Buffer data size in bytes (query only). */
#define AL_SIZE 0x2004

/* Buffer state. Not for public use. */
#define AL_UNUSED 0x2010
#define AL_PENDING 0x2011
#define AL_PROCESSED 0x2012

/** No error. */
#define AL_NO_ERROR 0

/** Invalid name (ID) passed to an AL call. */
#define AL_INVALID_NAME 0xA001

/** Invalid enumeration passed to AL call. */
#define AL_INVALID_ENUM 0xA002

/** Invalid value passed to AL call. */
#define AL_INVALID_VALUE 0xA003

/** Illegal AL call. */
#define AL_INVALID_OPERATION 0xA004

/** Not enough memory to execute the AL call. */
#define AL_OUT_OF_MEMORY 0xA005

/** Context string: Vendor name. */
#define AL_VENDOR 0xB001
/** Context string: Version. */
#define AL_VERSION 0xB002
/** Context string: Renderer name. */
#define AL_RENDERER 0xB003
/** Context string: Space-separated extension list. */
#define AL_EXTENSIONS 0xB004

/**
 * Doppler scale.
 * Type:    ALfloat
 * Range:   [0.0 - ]
 * Default: 1.0
 *
 * Scale for source and listener velocities.
 */
#define AL_DOPPLER_FACTOR 0xC000

/**
 * Doppler velocity (deprecated).
 *
 * A multiplier applied to the Speed of Sound.
 */
#define AL_DOPPLER_VELOCITY 0xC001

/**
 * Speed of Sound, in units per second.
 * Type:    ALfloat
 * Range:   [0.0001 - ]
 * Default: 343.3
 *
 * The speed at which sound waves are assumed to travel, when calculating the
 * doppler effect from source and listener velocities.
 */
#define AL_SPEED_OF_SOUND 0xC003

/**
 * Distance attenuation model.
 * Type:    ALenum
 * Range:   [AL_NONE, AL_INVERSE_DISTANCE, AL_INVERSE_DISTANCE_CLAMPED,
 *           AL_LINEAR_DISTANCE, AL_LINEAR_DISTANCE_CLAMPED,
 *           AL_EXPONENT_DISTANCE, AL_EXPONENT_DISTANCE_CLAMPED]
 * Default: AL_INVERSE_DISTANCE_CLAMPED
 *
 * The model by which sources attenuate with distance.
 *
 * None     - No distance attenuation.
 * Inverse  - Doubling the distance halves the source gain.
 * Linear   - Linear gain scaling between the reference and max distances.
 * Exponent - Exponential gain dropoff.
 *
 * Clamped variations work like the non-clamped counterparts, except the
 * distance calculated is clamped between the reference and max distances.
 */
#define AL_DISTANCE_MODEL 0xD000

/* Distance model values. */
#define AL_INVERSE_DISTANCE 0xD001
#define AL_INVERSE_DISTANCE_CLAMPED 0xD002
#define AL_LINEAR_DISTANCE 0xD003
#define AL_LINEAR_DISTANCE_CLAMPED 0xD004
#define AL_EXPONENT_DISTANCE 0xD005
#define AL_EXPONENT_DISTANCE_CLAMPED 0xD006

#ifndef AL_NO_PROTOTYPES
/* Renderer State management. */
AL_API void AL_APIENTRY alEnable(ALenum capability);
AL_API void AL_APIENTRY alDisable(ALenum capability);
AL_API ALboolean AL_APIENTRY alIsEnabled(ALenum capability);

/* Context state setting. */
AL_API void AL_APIENTRY alDopplerFactor(ALfloat value);
AL_API void AL_APIENTRY alDopplerVelocity(ALfloat value);
AL_API void AL_APIENTRY alSpeedOfSound(ALfloat value);
AL_API void AL_APIENTRY alDistanceModel(ALenum distanceModel);

/* Context state retrieval. */
AL_API const ALchar *AL_APIENTRY alGetString(ALenum param);
AL_API void AL_APIENTRY alGetBooleanv(ALenum param, ALboolean *values);
AL_API void AL_APIENTRY alGetIntegerv(ALenum param, ALint *values);
AL_API void AL_APIENTRY alGetFloatv(ALenum param, ALfloat *values);
AL_API void AL_APIENTRY alGetDoublev(ALenum param, ALdouble *values);
AL_API ALboolean AL_APIENTRY alGetBoolean(ALenum param);
AL_API ALint AL_APIENTRY alGetInteger(ALenum param);
AL_API ALfloat AL_APIENTRY alGetFloat(ALenum param);
AL_API ALdouble AL_APIENTRY alGetDouble(ALenum param);

/**
 * Obtain the first error generated in the AL context since the last call to
 * this function.
 */
AL_API ALenum AL_APIENTRY alGetError(void);

/** Query for the presence of an extension on the AL context. */
AL_API ALboolean AL_APIENTRY alIsExtensionPresent(const ALchar *extname);
/**
 * Retrieve the address of a function. The returned function may be context-
 * specific.
 */
AL_API void *AL_APIENTRY alGetProcAddress(const ALchar *fname);
/**
 * Retrieve the value of an enum. The returned value may be context-specific.
 */
AL_API ALenum AL_APIENTRY alGetEnumValue(const ALchar *ename);

/* Set listener parameters. */
AL_API void AL_APIENTRY alListenerf(ALenum param, ALfloat value);
AL_API void AL_APIENTRY alListener3f(ALenum param, ALfloat value1,
                                     ALfloat value2, ALfloat value3);
AL_API void AL_APIENTRY alListenerfv(ALenum param, const ALfloat *values);
AL_API void AL_APIENTRY alListeneri(ALenum param, ALint value);
AL_API void AL_APIENTRY alListener3i(ALenum param, ALint value1, ALint value2,
                                     ALint value3);
AL_API void AL_APIENTRY alListeneriv(ALenum param, const ALint *values);

/* Get listener parameters. */
AL_API void AL_APIENTRY alGetListenerf(ALenum param, ALfloat *value);
AL_API void AL_APIENTRY alGetListener3f(ALenum param, ALfloat *value1,
                                        ALfloat *value2, ALfloat *value3);
AL_API void AL_APIENTRY alGetListenerfv(ALenum param, ALfloat *values);
AL_API void AL_APIENTRY alGetListeneri(ALenum param, ALint *value);
AL_API void AL_APIENTRY alGetListener3i(ALenum param, ALint *value1,
                                        ALint *value2, ALint *value3);
AL_API void AL_APIENTRY alGetListeneriv(ALenum param, ALint *values);

/** Create source objects. */
AL_API void AL_APIENTRY alGenSources(ALsizei n, ALuint *sources);
/** Delete source objects. */
AL_API void AL_APIENTRY alDeleteSources(ALsizei n, const ALuint *sources);
/** Verify an ID is for a valid source. */
AL_API ALboolean AL_APIENTRY alIsSource(ALuint source);

/* Set source parameters. */
AL_API void AL_APIENTRY alSourcef(ALuint source, ALenum param, ALfloat value);
AL_API void AL_APIENTRY alSource3f(ALuint source, ALenum param, ALfloat value1,
                                   ALfloat value2, ALfloat value3);
AL_API void AL_APIENTRY alSourcefv(ALuint source, ALenum param,
                                   const ALfloat *values);
AL_API void AL_APIENTRY alSourcei(ALuint source, ALenum param, ALint value);
AL_API void AL_APIENTRY alSource3i(ALuint source, ALenum param, ALint value1,
                                   ALint value2, ALint value3);
AL_API void AL_APIENTRY alSourceiv(ALuint source, ALenum param,
                                   const ALint *values);

/* Get source parameters. */
AL_API void AL_APIENTRY alGetSourcef(ALuint source, ALenum param,
                                     ALfloat *value);
AL_API void AL_APIENTRY alGetSource3f(ALuint source, ALenum param,
                                      ALfloat *value1, ALfloat *value2,
                                      ALfloat *value3);
AL_API void AL_APIENTRY alGetSourcefv(ALuint source, ALenum param,
                                      ALfloat *values);
AL_API void AL_APIENTRY alGetSourcei(ALuint source, ALenum param, ALint *value);
AL_API void AL_APIENTRY alGetSource3i(ALuint source, ALenum param,
                                      ALint *value1, ALint *value2,
                                      ALint *value3);
AL_API void AL_APIENTRY alGetSourceiv(ALuint source, ALenum param,
                                      ALint *values);

/** Play, restart, or resume a source, setting its state to AL_PLAYING. */
AL_API void AL_APIENTRY alSourcePlay(ALuint source);
/** Stop a source, setting its state to AL_STOPPED if playing or paused. */
AL_API void AL_APIENTRY alSourceStop(ALuint source);
/** Rewind a source, setting its state to AL_INITIAL. */
AL_API void AL_APIENTRY alSourceRewind(ALuint source);
/** Pause a source, setting its state to AL_PAUSED if playing. */
AL_API void AL_APIENTRY alSourcePause(ALuint source);

/** Play, restart, or resume a list of sources atomically. */
AL_API void AL_APIENTRY alSourcePlayv(ALsizei n, const ALuint *sources);
/** Stop a list of sources atomically. */
AL_API void AL_APIENTRY alSourceStopv(ALsizei n, const ALuint *sources);
/** Rewind a list of sources atomically. */
AL_API void AL_APIENTRY alSourceRewindv(ALsizei n, const ALuint *sources);
/** Pause a list of sources atomically. */
AL_API void AL_APIENTRY alSourcePausev(ALsizei n, const ALuint *sources);

/** Queue buffers onto a source */
AL_API void AL_APIENTRY alSourceQueueBuffers(ALuint source, ALsizei nb,
                                             const ALuint *buffers);
/** Unqueue processed buffers from a source */
AL_API void AL_APIENTRY alSourceUnqueueBuffers(ALuint source, ALsizei nb,
                                               ALuint *buffers);

/** Create buffer objects */
AL_API void AL_APIENTRY alGenBuffers(ALsizei n, ALuint *buffers);
/** Delete buffer objects */
AL_API void AL_APIENTRY alDeleteBuffers(ALsizei n, const ALuint *buffers);
/** Verify an ID is a valid buffer (including the NULL buffer) */
AL_API ALboolean AL_APIENTRY alIsBuffer(ALuint buffer);

/**
 * Copies data into the buffer, interpreting it using the specified format and
 * samplerate.
 */
AL_API void AL_APIENTRY alBufferData(ALuint buffer, ALenum format,
                                     const ALvoid *data, ALsizei size,
                                     ALsizei samplerate);

/* Set buffer parameters. */
AL_API void AL_APIENTRY alBufferf(ALuint buffer, ALenum param, ALfloat value);
AL_API void AL_APIENTRY alBuffer3f(ALuint buffer, ALenum param, ALfloat value1,
                                   ALfloat value2, ALfloat value3);
AL_API void AL_APIENTRY alBufferfv(ALuint buffer, ALenum param,
                                   const ALfloat *values);
AL_API void AL_APIENTRY alBufferi(ALuint buffer, ALenum param, ALint value);
AL_API void AL_APIENTRY alBuffer3i(ALuint buffer, ALenum param, ALint value1,
                                   ALint value2, ALint value3);
AL_API void AL_APIENTRY alBufferiv(ALuint buffer, ALenum param,
                                   const ALint *values);

/* Get buffer parameters. */
AL_API void AL_APIENTRY alGetBufferf(ALuint buffer, ALenum param,
                                     ALfloat *value);
AL_API void AL_APIENTRY alGetBuffer3f(ALuint buffer, ALenum param,
                                      ALfloat *value1, ALfloat *value2,
                                      ALfloat *value3);
AL_API void AL_APIENTRY alGetBufferfv(ALuint buffer, ALenum param,
                                      ALfloat *values);
AL_API void AL_APIENTRY alGetBufferi(ALuint buffer, ALenum param, ALint *value);
AL_API void AL_APIENTRY alGetBuffer3i(ALuint buffer, ALenum param,
                                      ALint *value1, ALint *value2,
                                      ALint *value3);
AL_API void AL_APIENTRY alGetBufferiv(ALuint buffer, ALenum param,
                                      ALint *values);
#endif /* AL_NO_PROTOTYPES */

/* Pointer-to-function types, useful for storing dynamically loaded AL entry
 * points.
 */
typedef void(AL_APIENTRY *LPALENABLE)(ALenum capability);
typedef void(AL_APIENTRY *LPALDISABLE)(ALenum capability);
typedef ALboolean(AL_APIENTRY *LPALISENABLED)(ALenum capability);
typedef const ALchar *(AL_APIENTRY *LPALGETSTRING)(ALenum param);
typedef void(AL_APIENTRY *LPALGETBOOLEANV)(ALenum param, ALboolean *values);
typedef void(AL_APIENTRY *LPALGETINTEGERV)(ALenum param, ALint *values);
typedef void(AL_APIENTRY *LPALGETFLOATV)(ALenum param, ALfloat *values);
typedef void(AL_APIENTRY *LPALGETDOUBLEV)(ALenum param, ALdouble *values);
typedef ALboolean(AL_APIENTRY *LPALGETBOOLEAN)(ALenum param);
typedef ALint(AL_APIENTRY *LPALGETINTEGER)(ALenum param);
typedef ALfloat(AL_APIENTRY *LPALGETFLOAT)(ALenum param);
typedef ALdouble(AL_APIENTRY *LPALGETDOUBLE)(ALenum param);
typedef ALenum(AL_APIENTRY *LPALGETERROR)(void);
typedef ALboolean(AL_APIENTRY *LPALISEXTENSIONPRESENT)(const ALchar *extname);
typedef void *(AL_APIENTRY *LPALGETPROCADDRESS)(const ALchar *fname);
typedef ALenum(AL_APIENTRY *LPALGETENUMVALUE)(const ALchar *ename);
typedef void(AL_APIENTRY *LPALLISTENERF)(ALenum param, ALfloat value);
typedef void(AL_APIENTRY *LPALLISTENER3F)(ALenum param, ALfloat value1,
                                          ALfloat value2, ALfloat value3);
typedef void(AL_APIENTRY *LPALLISTENERFV)(ALenum param, const ALfloat *values);
typedef void(AL_APIENTRY *LPALLISTENERI)(ALenum param, ALint value);
typedef void(AL_APIENTRY *LPALLISTENER3I)(ALenum param, ALint value1,
                                          ALint value2, ALint value3);
typedef void(AL_APIENTRY *LPALLISTENERIV)(ALenum param, const ALint *values);
typedef void(AL_APIENTRY *LPALGETLISTENERF)(ALenum param, ALfloat *value);
typedef void(AL_APIENTRY *LPALGETLISTENER3F)(ALenum param, ALfloat *value1,
                                             ALfloat *value2, ALfloat *value3);
typedef void(AL_APIENTRY *LPALGETLISTENERFV)(ALenum param, ALfloat *values);
typedef void(AL_APIENTRY *LPALGETLISTENERI)(ALenum param, ALint *value);
typedef void(AL_APIENTRY *LPALGETLISTENER3I)(ALenum param, ALint *value1,
                                             ALint *value2, ALint *value3);
typedef void(AL_APIENTRY *LPALGETLISTENERIV)(ALenum param, ALint *values);
typedef void(AL_APIENTRY *LPALGENSOURCES)(ALsizei n, ALuint *sources);
typedef void(AL_APIENTRY *LPALDELETESOURCES)(ALsizei n, const ALuint *sources);
typedef ALboolean(AL_APIENTRY *LPALISSOURCE)(ALuint source);
typedef void(AL_APIENTRY *LPALSOURCEF)(ALuint source, ALenum param,
                                       ALfloat value);
typedef void(AL_APIENTRY *LPALSOURCE3F)(ALuint source, ALenum param,
                                        ALfloat value1, ALfloat value2,
                                        ALfloat value3);
typedef void(AL_APIENTRY *LPALSOURCEFV)(ALuint source, ALenum param,
                                        const ALfloat *values);
typedef void(AL_APIENTRY *LPALSOURCEI)(ALuint source, ALenum param,
                                       ALint value);
typedef void(AL_APIENTRY *LPALSOURCE3I)(ALuint source, ALenum param,
                                        ALint value1, ALint value2,
                                        ALint value3);
typedef void(AL_APIENTRY *LPALSOURCEIV)(ALuint source, ALenum param,
                                        const ALint *values);
typedef void(AL_APIENTRY *LPALGETSOURCEF)(ALuint source, ALenum param,
                                          ALfloat *value);
typedef void(AL_APIENTRY *LPALGETSOURCE3F)(ALuint source, ALenum param,
                                           ALfloat *value1, ALfloat *value2,
                                           ALfloat *value3);
typedef void(AL_APIENTRY *LPALGETSOURCEFV)(ALuint source, ALenum param,
                                           ALfloat *values);
typedef void(AL_APIENTRY *LPALGETSOURCEI)(ALuint source, ALenum param,
                                          ALint *value);
typedef void(AL_APIENTRY *LPALGETSOURCE3I)(ALuint source, ALenum param,
                                           ALint *value1, ALint *value2,
                                           ALint *value3);
typedef void(AL_APIENTRY *LPALGETSOURCEIV)(ALuint source, ALenum param,
                                           ALint *values);
typedef void(AL_APIENTRY *LPALSOURCEPLAYV)(ALsizei n, const ALuint *sources);
typedef void(AL_APIENTRY *LPALSOURCESTOPV)(ALsizei n, const ALuint *sources);
typedef void(AL_APIENTRY *LPALSOURCEREWINDV)(ALsizei n, const ALuint *sources);
typedef void(AL_APIENTRY *LPALSOURCEPAUSEV)(ALsizei n, const ALuint *sources);
typedef void(AL_APIENTRY *LPALSOURCEPLAY)(ALuint source);
typedef void(AL_APIENTRY *LPALSOURCESTOP)(ALuint source);
typedef void(AL_APIENTRY *LPALSOURCEREWIND)(ALuint source);
typedef void(AL_APIENTRY *LPALSOURCEPAUSE)(ALuint source);
typedef void(AL_APIENTRY *LPALSOURCEQUEUEBUFFERS)(ALuint source, ALsizei nb,
                                                  const ALuint *buffers);
typedef void(AL_APIENTRY *LPALSOURCEUNQUEUEBUFFERS)(ALuint source, ALsizei nb,
                                                    ALuint *buffers);
typedef void(AL_APIENTRY *LPALGENBUFFERS)(ALsizei n, ALuint *buffers);
typedef void(AL_APIENTRY *LPALDELETEBUFFERS)(ALsizei n, const ALuint *buffers);
typedef ALboolean(AL_APIENTRY *LPALISBUFFER)(ALuint buffer);
typedef void(AL_APIENTRY *LPALBUFFERDATA)(ALuint buffer, ALenum format,
                                          const ALvoid *data, ALsizei size,
                                          ALsizei samplerate);
typedef void(AL_APIENTRY *LPALBUFFERF)(ALuint buffer, ALenum param,
                                       ALfloat value);
typedef void(AL_APIENTRY *LPALBUFFER3F)(ALuint buffer, ALenum param,
                                        ALfloat value1, ALfloat value2,
                                        ALfloat value3);
typedef void(AL_APIENTRY *LPALBUFFERFV)(ALuint buffer, ALenum param,
                                        const ALfloat *values);
typedef void(AL_APIENTRY *LPALBUFFERI)(ALuint buffer, ALenum param,
                                       ALint value);
typedef void(AL_APIENTRY *LPALBUFFER3I)(ALuint buffer, ALenum param,
                                        ALint value1, ALint value2,
                                        ALint value3);
typedef void(AL_APIENTRY *LPALBUFFERIV)(ALuint buffer, ALenum param,
                                        const ALint *values);
typedef void(AL_APIENTRY *LPALGETBUFFERF)(ALuint buffer, ALenum param,
                                          ALfloat *value);
typedef void(AL_APIENTRY *LPALGETBUFFER3F)(ALuint buffer, ALenum param,
                                           ALfloat *value1, ALfloat *value2,
                                           ALfloat *value3);
typedef void(AL_APIENTRY *LPALGETBUFFERFV)(ALuint buffer, ALenum param,
                                           ALfloat *values);
typedef void(AL_APIENTRY *LPALGETBUFFERI)(ALuint buffer, ALenum param,
                                          ALint *value);
typedef void(AL_APIENTRY *LPALGETBUFFER3I)(ALuint buffer, ALenum param,
                                           ALint *value1, ALint *value2,
                                           ALint *value3);
typedef void(AL_APIENTRY *LPALGETBUFFERIV)(ALuint buffer, ALenum param,
                                           ALint *values);
typedef void(AL_APIENTRY *LPALDOPPLERFACTOR)(ALfloat value);
typedef void(AL_APIENTRY *LPALDOPPLERVELOCITY)(ALfloat value);
typedef void(AL_APIENTRY *LPALSPEEDOFSOUND)(ALfloat value);
typedef void(AL_APIENTRY *LPALDISTANCEMODEL)(ALenum distanceModel);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AL_AL_H */
