{
  'targets': [
    {
      'target_name': '<(module_name)',
      'include_dirs': [ 
        # "<!(node -e \"require('nan')\")",
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      'sources': [
        'src/main.cpp',
        'src/diskusage.h',
      ],
      'cflags' : [
        '-std=c++11',
        '-D_GLIBCXX_USE_CXX11_ABI=0'
      ],
      'conditions': [
        ['OS=="win"', {
          'sources': [
            'src/diskusage_win.cpp',
          ],
          'defines': [
            'UNICODE',
            'WIN32_LEAN_AND_MEAN',
            'NOMINMAX'
          ],
          'msvs_settings': {
            'VCCLCompilerTool': {
              'ExceptionHandling': '1'
            }
          }
        }],
        ['OS not in ["win"]', {
          'sources': [
            'src/diskusage_posix.cpp',
          ],
          'cflags!': [ '-fno-exceptions' ],
          'cflags_cc!': [ '-fno-exceptions' ]
        }],
        ['OS=="mac"', {
          'cflags+': ['-fvisibility=hidden'],
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'CLANG_CXX_LIBRARY': 'libc++',
            'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES', # -fvisibility=hidden
          }
        }]
      ]
    },
    {
      "target_name": "action_after_build",
      "type": "none",
      "dependencies": [ "<(module_name)" ],
      "copies": [
          {
            "files": [ "<(PRODUCT_DIR)/<(module_name).node" ],
            "destination": "<(module_path)"
          }
      ]
    }
  ]
}
