{
  'targets': [
    {
      'target_name': 'screencapture',
      'sources': [
        'src/qt.cc',
        'src/QtGui/qapplication.cc',
        'src/sc/fullscreenimageeditor.cpp',
        'src/sc/moc_fullscreenimageeditor.cpp',
        'src/sc/configmanager.cpp'
        #'src/test/hellowindow.cpp',
        #'src/test/moc_hellowindow.cpp'
      ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '-mmacosx-version-min=10.7',
              '-std=c++11',
              '-stdlib=libc++',
              #'-D__STDC_CONSTANT_MACROS',
              #'-D_FILE_OFFSET_BITS=64',
              #'-D_LARGEFILE_SOURCE',
              #'-Wall'
            ],
            #'OTHER_CPLUSPLUSFLAGS': [
            #  '-mmacosx-version-min=10.7',
            #  '-std=c++11',
            #  '-stdlib=libc++',
            #  '-D__STDC_CONSTANT_MACROS',
            #  '-D_FILE_OFFSET_BITS=64',
            #  '-D_LARGEFILE_SOURCE',
            #  '-Wall'
            #],
            #'MACOSX_DEPLOYMENT_TARGET': '10.7',
            'GCC_ENABLE_CPP_RTTI': 'YES',
            #'GCC_ENABLE_CPP_EXCEPTIONS': 'YES'
          },
          'include_dirs': [
            #'<!(node -e \"require(\'nan\')\")',
            'deps/qt-5.7.0/darwin/x64/include',
            'deps/qt-5.7.0/darwin/x64/include/QtCore',
            'deps/qt-5.7.0/darwin/x64/include/QtGui',
            'deps/qt-5.7.0/darwin/x64/include/QtWidgets'
          ],
          'libraries': [
            # TODO: fix node-gyp behavior that requires ../
            '../deps/qt-5.7.0/darwin/x64/lib/QtCore',
            '../deps/qt-5.7.0/darwin/x64/lib/QtGui',
            '../deps/qt-5.7.0/darwin/x64/lib/QtWidgets',
            #libqcocoa.dylib needs QtDBus and QtPrintSupport
            '../deps/qt-5.7.0/darwin/x64/lib/QtDBus',
            '../deps/qt-5.7.0/darwin/x64/lib/QtPrintSupport'
          ]
          #"link_settings": {
          #  "libraries": [
          #    "-libqcocoa.dylib"
          #  ],
          #}
        }],
        ['OS=="linux"', {
          'cflags': [
            '<!@(pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets Qt5Multimedia)'
          ],
          'ldflags': [
            '<!@(pkg-config --libs-only-L --libs-only-other Qt5Core Qt5Gui Qt5Widgets Qt5Multimedia)'
          ],
          'libraries': [
            '<!@(pkg-config --libs-only-l Qt5Core Qt5Gui Qt5Widgets Qt5Multimedia)'
          ]
        }],
        ['OS=="win"', {
		  'cflags': [
                '/GR',
           ],
		   'configurations': {
              'Debug': {
                'msvs_settings': {
                  'VCCLCompilerTool': {
                    'RuntimeTypeInfo': 'true',
                    'AdditionalOptions': ['/GR'], 
                  }
                }
              },
              'Release': {                            
                'msvs_settings': {
                  'VCCLCompilerTool': {
                    'RuntimeTypeInfo': 'true',
                    'AdditionalOptions': ['/GR'],
                  }
                }
              }
            },
          'include_dirs': [
              #'<!(node -e \"require('nan')\")',
              'deps/qt-5.7.0/win32/ia32/include',
              'deps/qt-5.7.0/win32/ia32/include/QtCore',
              'deps/qt-5.7.0/win32/ia32/include/QtGui',
              'deps/qt-5.7.0/win32/ia32/include/QtWidgets',
          ],
          'libraries': [
              # TODO: fix node-gyp behavior that requires ../
              '../deps/qt-5.7.0/win32/ia32/lib/Qt5Core.lib',
              '../deps/qt-5.7.0/win32/ia32/lib/Qt5Gui.lib',
              '../deps/qt-5.7.0/win32/ia32/lib/Qt5Widgets.lib'
          ],
        }]
      ]
    },
    {
      'target_name': 'copy_binary',
      'type': 'none',
      'dependencies': ['screencapture'],
      'conditions': [
        ['OS=="win"', {
          'copies':
          [
            {
              'destination': 'build/Release/',
              'files': [
                'deps/qt-5.7.0/win32/ia32/Qt5Widgets.dll',
                'deps/qt-5.7.0/win32/ia32/Qt5Core.dll',
                'deps/qt-5.7.0/win32/ia32/Qt5Gui.dll',
                #'deps/qt-5.7.0/win32/ia32/Qt5Multimedia.dll',
                #'deps/qt-5.7.0/win32/ia32/Qt5Network.dll',
                #'deps/qt-5.7.0/win32/ia32/libGLESv2.dll',
                #'deps/qt-5.7.0/win32/ia32/icudt52.dll',
                #'deps/qt-5.7.0/win32/ia32/icuin52.dll',
                #'deps/qt-5.7.0/win32/ia32/icuuc52.dll',
              ]
            }
          ]
        }]
      ]
    }
  ]
}
