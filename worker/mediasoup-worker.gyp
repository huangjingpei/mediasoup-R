{
  'target_defaults': {
    'type': 'executable',
    'dependencies':
    [
      'deps/netstring/netstring.gyp:netstring',
      'deps/libuv/uv.gyp:libuv'
    ],
    # TODO: SCTP_DEBUG must be dynamic based on a condition variable in common.gyp.
    # 'defines': [ 'SCTP_DEBUG' ],
    'sources':
    [
      # C++ source files.
      'src/DepLibUV.cpp',
      'src/Logger.cpp',
      'src/Settings.cpp',
      'src/Worker.cpp',
      'src/Utils/File.cpp',
      'src/Utils/IP.cpp',
      'src/WebSocketClient.cpp',
      'src/RecordClient.cpp',
      'src/handles/SignalsHandler.cpp',
      'src/handles/TcpConnection.cpp',
      'src/handles/TcpServer.cpp',
      'src/handles/Timer.cpp',
      'src/handles/UdpSocket.cpp',
      'src/handles/UnixStreamSocket.cpp',
      'src/Channel/Notifier.cpp',
      'src/Channel/Request.cpp',
      'src/Channel/UnixStreamSocket.cpp',
      # C++ include files.
      'include/DepLibUV.hpp',
      'include/DepOpenSSL.hpp',
      'include/LogLevel.hpp',
      'include/Logger.hpp',
      'include/MediaSoupErrors.hpp',
      'include/Settings.hpp',
      'include/Utils.hpp',
      'include/Worker.hpp',
      'include/common.hpp',
      'src/WebSocketClient.hpp',
      'src/RecordClient.hpp',
      'include/handles/SignalsHandler.hpp',
      'include/handles/TcpConnection.hpp',
      'include/handles/TcpServer.hpp',
      'include/handles/Timer.hpp',
      'include/handles/UdpSocket.hpp',
      'include/handles/UnixStreamSocket.hpp',
      'include/Channel/Notifier.hpp',
      'include/Channel/Request.hpp',
      'include/Channel/UnixStreamSocket.hpp',
    ],
    'include_dirs':
    [
      'include'
    ],
    'conditions':
    [
      # FIPS.
      [ 'openssl_fips != ""', {
        'defines': [ 'BUD_FIPS_ENABLED=1' ]
      }],

      # Endianness.
      [ 'node_byteorder == "big"', {
          # Define Big Endian.
          'defines': [ 'MS_BIG_ENDIAN' ]
        }, {
          # Define Little Endian.
          'defines': [ 'MS_LITTLE_ENDIAN' ]
      }],

      # Platform-specifics.

      [ 'OS == "mac" and mediasoup_asan == "true"', {
        'xcode_settings':
        {
          'OTHER_CFLAGS': [ '-fsanitize=address' ],
          'OTHER_LDFLAGS': [ '-fsanitize=address' ]
        }
      }],

      [ 'OS == "linux"', {
        'defines':
        [
          '_POSIX_C_SOURCE=200112',
          '_GNU_SOURCE'
        ]
      }],

      [ 'OS == "linux" and mediasoup_asan == "true"', {
        'cflags': [ '-fsanitize=address' ],
        'ldflags': [ '-fsanitize=address' ]
      }],

      [ 'OS in "linux freebsd"', {
        'ldflags': [ '-Wl,--whole-archive <(libopenssl) -Wl,--no-whole-archive' ]
      }],

      [ 'OS in "freebsd"', {
        'ldflags': [ '-Wl,--export-dynamic' ]
      }],

      [ 'OS != "win"', {
        'cflags': [ '-std=c++11', '-Wall', '-Wextra', '-Wno-unused-parameter', '-Wno-implicit-fallthrough' ]
      }],

      [ 'OS == "mac"', {
        'xcode_settings':
        {
          'WARNING_CFLAGS': [ '-Wall', '-Wextra', '-Wno-unused-parameter' ],
          'OTHER_CPLUSPLUSFLAGS' : [ '-std=c++11' ]
        }
      }],

      # Dependency-specifics.

      [ 'sctp_debug == "true"', {
        'defines': [ 'SCTP_DEBUG' ]
      }]
    ]
  },
  'targets':
  [
    {
      'target_name': 'mediasoup-worker',
      'sources':
      [
        # C++ source files.
        'src/main.cpp'
      ]
    }
  ]
}
