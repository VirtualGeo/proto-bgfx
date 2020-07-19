
/*
 * Copyright 2011-2020 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <bx/bx.h>
#include <bx/file.h>
#include <bx/sort.h>
#include <bgfx/bgfx.h>

#include <time.h>

#if BX_PLATFORM_EMSCRIPTEN
#	include <emscripten.h>
#endif // BX_PLATFORM_EMSCRIPTEN

#include "entry_p.h"
#include "cmd.h"

extern "C" int32_t _main_(int32_t _argc, char** _argv);

namespace entry
{
	static uint32_t s_debug = BGFX_DEBUG_NONE;
	static uint32_t s_reset = BGFX_RESET_NONE;
	static uint32_t s_width = ENTRY_DEFAULT_WIDTH;
	static uint32_t s_height = ENTRY_DEFAULT_HEIGHT;
	static bool s_exit = false;

	static bx::FileReaderI* s_fileReader = NULL;
	static bx::FileWriterI* s_fileWriter = NULL;

	extern bx::AllocatorI* getDefaultAllocator();
	bx::AllocatorI* g_allocator = getDefaultAllocator();


	typedef bx::StringT<&g_allocator> String;

	static String s_currentDir;

	class FileReader : public bx::FileReader
	{
		typedef bx::FileReader super;

	public:
		virtual bool open(const bx::FilePath& _filePath, bx::Error* _err) override
		{
			String filePath(s_currentDir);
			filePath.append(_filePath);
			return super::open(filePath.getPtr(), _err);
		}
	};

	class FileWriter : public bx::FileWriter
	{
		typedef bx::FileWriter super;

	public:
		virtual bool open(const bx::FilePath& _filePath, bool _append, bx::Error* _err) override
		{
			String filePath(s_currentDir);
			filePath.append(_filePath);
			return super::open(filePath.getPtr(), _append, _err);
		}
	};

    void initFileReaderWriter() {
        s_fileReader = BX_NEW(g_allocator, FileReader);
        s_fileWriter = BX_NEW(g_allocator, FileWriter);
    }

	void setCurrentDir(const char* _dir)
	{
		s_currentDir.set(_dir);
	}

#if ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR
	bx::AllocatorI* getDefaultAllocator()
	{
BX_PRAGMA_DIAGNOSTIC_PUSH();
BX_PRAGMA_DIAGNOSTIC_IGNORED_MSVC(4459); // warning C4459: declaration of 's_allocator' hides global declaration
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG_GCC("-Wshadow");
		static bx::DefaultAllocator s_allocator;
		return &s_allocator;
BX_PRAGMA_DIAGNOSTIC_POP();
	}
#endif // ENTRY_CONFIG_IMPLEMENT_DEFAULT_ALLOCATOR

	static const char* s_keyName[] =
	{
		"None",
		"Esc",
		"Return",
		"Tab",
		"Space",
		"Backspace",
		"Up",
		"Down",
		"Left",
		"Right",
		"Insert",
		"Delete",
		"Home",
		"End",
		"PageUp",
		"PageDown",
		"Print",
		"Plus",
		"Minus",
		"LeftBracket",
		"RightBracket",
		"Semicolon",
		"Quote",
		"Comma",
		"Period",
		"Slash",
		"Backslash",
		"Tilde",
		"F1",
		"F2",
		"F3",
		"F4",
		"F5",
		"F6",
		"F7",
		"F8",
		"F9",
		"F10",
		"F11",
		"F12",
		"NumPad0",
		"NumPad1",
		"NumPad2",
		"NumPad3",
		"NumPad4",
		"NumPad5",
		"NumPad6",
		"NumPad7",
		"NumPad8",
		"NumPad9",
		"Key0",
		"Key1",
		"Key2",
		"Key3",
		"Key4",
		"Key5",
		"Key6",
		"Key7",
		"Key8",
		"Key9",
		"KeyA",
		"KeyB",
		"KeyC",
		"KeyD",
		"KeyE",
		"KeyF",
		"KeyG",
		"KeyH",
		"KeyI",
		"KeyJ",
		"KeyK",
		"KeyL",
		"KeyM",
		"KeyN",
		"KeyO",
		"KeyP",
		"KeyQ",
		"KeyR",
		"KeyS",
		"KeyT",
		"KeyU",
		"KeyV",
		"KeyW",
		"KeyX",
		"KeyY",
		"KeyZ",
		"GamepadA",
		"GamepadB",
		"GamepadX",
		"GamepadY",
		"GamepadThumbL",
		"GamepadThumbR",
		"GamepadShoulderL",
		"GamepadShoulderR",
		"GamepadUp",
		"GamepadDown",
		"GamepadLeft",
		"GamepadRight",
		"GamepadBack",
		"GamepadStart",
		"GamepadGuide",
	};
	BX_STATIC_ASSERT(Key::Count == BX_COUNTOF(s_keyName) );

	const char* getName(Key::Enum _key)
	{
		BX_CHECK(_key < Key::Count, "Invalid key %d.", _key);
		return s_keyName[_key];
	}

	char keyToAscii(Key::Enum _key, uint8_t _modifiers)
	{
		const bool isAscii = (Key::Key0 <= _key && _key <= Key::KeyZ)
						  || (Key::Esc  <= _key && _key <= Key::Minus);
		if (!isAscii)
		{
			return '\0';
		}

		const bool isNumber = (Key::Key0 <= _key && _key <= Key::Key9);
		if (isNumber)
		{
			return '0' + char(_key - Key::Key0);
		}

		const bool isChar = (Key::KeyA <= _key && _key <= Key::KeyZ);
		if (isChar)
		{
			enum { ShiftMask = Modifier::LeftShift|Modifier::RightShift };

			const bool shift = !!(_modifiers&ShiftMask);
			return (shift ? 'A' : 'a') + char(_key - Key::KeyA);
		}

		switch (_key)
		{
		case Key::Esc:       return 0x1b;
		case Key::Return:    return '\n';
		case Key::Tab:       return '\t';
		case Key::Space:     return ' ';
		case Key::Backspace: return 0x08;
		case Key::Plus:      return '+';
		case Key::Minus:     return '-';
		default:             break;
		}

		return '\0';
	}

	bool setOrToggle(uint32_t& _flags, const char* _name, uint32_t _bit, int _first, int _argc, char const* const* _argv)
	{
		if (0 == bx::strCmp(_argv[_first], _name) )
		{
			int arg = _first+1;
			if (_argc > arg)
			{
				_flags &= ~_bit;

				bool set = false;
				bx::fromString(&set, _argv[arg]);

				_flags |= set ? _bit : 0;
			}
			else
			{
				_flags ^= _bit;
			}

			return true;
		}

		return false;
	}


	int cmdExit(CmdContext* /*_context*/, void* /*_userData*/, int /*_argc*/, char const* const* /*_argv*/)
	{
		s_exit = true;
		return bx::kExitSuccess;
	}


#if BX_PLATFORM_EMSCRIPTEN
	static AppI* s_app;
	static void updateApp()
	{
		s_app->update();
	}
#endif // BX_PLATFORM_EMSCRIPTEN

	static AppI*    s_currentApp = NULL;
	static AppI*    s_apps       = NULL;
	static uint32_t s_numApps    = 0;

	static char s_restartArgs[1024] = { '\0' };

	static AppI* getCurrentApp(AppI* _set = NULL)
	{
		if (NULL != _set)
		{
			s_currentApp = _set;
		}
		else if (NULL == s_currentApp)
		{
			s_currentApp = getFirstApp();
		}

		return s_currentApp;
	}

	static AppI* getNextWrap(AppI* _app)
	{
		AppI* next = _app->getNext();
		if (NULL != next)
		{
			return next;
		}

		return getFirstApp();
	}

	int cmdApp(CmdContext* /*_context*/, void* /*_userData*/, int _argc, char const* const* _argv)
	{
		if (0 == bx::strCmp(_argv[1], "restart") )
		{
			if (2 == _argc)
			{
				bx::strCopy(s_restartArgs, BX_COUNTOF(s_restartArgs), getCurrentApp()->getName() );
				return bx::kExitSuccess;
			}

			if (0 == bx::strCmp(_argv[2], "next") )
			{
				AppI* next = getNextWrap(getCurrentApp() );
				bx::strCopy(s_restartArgs, BX_COUNTOF(s_restartArgs), next->getName() );
				return bx::kExitSuccess;
			}
			else if (0 == bx::strCmp(_argv[2], "prev") )
			{
				AppI* prev = getCurrentApp();
				for (AppI* app = getNextWrap(prev); app != getCurrentApp(); app = getNextWrap(app) )
				{
					prev = app;
				}

				bx::strCopy(s_restartArgs, BX_COUNTOF(s_restartArgs), prev->getName() );
				return bx::kExitSuccess;
			}

			for (AppI* app = getFirstApp(); NULL != app; app = app->getNext() )
			{
				if (0 == bx::strCmp(_argv[2], app->getName() ) )
				{
					bx::strCopy(s_restartArgs, BX_COUNTOF(s_restartArgs), app->getName() );
					return bx::kExitSuccess;
				}
			}
		}

		return bx::kExitFailure;
	}

	AppI::AppI(const char* _name, const char* _description, const char* _url)
	{
		m_name        = _name;
		m_description = _description;
		m_url         = _url;
		m_next        = s_apps;

		s_apps = this;
		s_numApps++;
	}

	AppI::~AppI()
	{
		for (AppI* prev = NULL, *app = s_apps, *next = app->getNext()
			; NULL != app
			; prev = app, app = next, next = app->getNext() )
		{
			if (app == this)
			{
				if (NULL != prev)
				{
					prev->m_next = next;
				}
				else
				{
					s_apps = next;
				}

				--s_numApps;

				break;
			}
		}
	}

	const char* AppI::getName() const
	{
		return m_name;
	}

	const char* AppI::getDescription() const
	{
		return m_description;
	}

	const char* AppI::getUrl() const
	{
		return m_url;
	}

	AppI* AppI::getNext()
	{
		return m_next;
	}

	AppI* getFirstApp()
	{
		return s_apps;
	}

	uint32_t getNumApps()
	{
		return s_numApps;
	}


	static int32_t sortApp(const void* _lhs, const void* _rhs)
	{
		const AppI* lhs = *(const AppI**)_lhs;
		const AppI* rhs = *(const AppI**)_rhs;

		return bx::strCmpI(lhs->getName(), rhs->getName() );
	}

	static void sortApps()
	{
		if (2 > s_numApps)
		{
			return;
		}

		AppI** apps = (AppI**)BX_ALLOC(g_allocator, s_numApps*sizeof(AppI*) );

		uint32_t ii = 0;
		for (AppI* app = getFirstApp(); NULL != app; app = app->getNext() )
		{
			apps[ii++] = app;
		}
		bx::quickSort(apps, s_numApps, sizeof(AppI*), sortApp);

		s_apps = apps[0];
		for (ii = 1; ii < s_numApps; ++ii)
		{
			AppI* app = apps[ii-1];
			app->m_next = apps[ii];
		}
		apps[s_numApps-1]->m_next = NULL;

		BX_FREE(g_allocator, apps);
	}


	WindowState s_window[ENTRY_CONFIG_MAX_WINDOWS];



	bx::FileReaderI* getFileReader()
	{
        if (s_fileReader == nullptr) {
            initFileReaderWriter();
        }
		return s_fileReader;
	}

	bx::FileWriterI* getFileWriter()
	{
        if (s_fileWriter == nullptr) {
            initFileReaderWriter();
        }
		return s_fileWriter;
	}

	bx::AllocatorI* getAllocator()
	{
		if (NULL == g_allocator)
		{
			g_allocator = getDefaultAllocator();
		}

		return g_allocator;
	}

	void* TinyStlAllocator::static_allocate(size_t _bytes)
	{
		return BX_ALLOC(getAllocator(), _bytes);
	}

	void TinyStlAllocator::static_deallocate(void* _ptr, size_t /*_bytes*/)
	{
		if (NULL != _ptr)
		{
			BX_FREE(getAllocator(), _ptr);
		}
	}

} // namespace entry
