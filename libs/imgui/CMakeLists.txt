mg_cc_library(
	NAME
		imgui
	TYPE
		STATIC
	SRCS
		imconfig.h
		imgui.cpp
		imgui.h
		imgui_demo.cpp
		imgui_draw.cpp
		imgui_internal.h
		imgui_widgets.cpp
		imstb_rectpack.h
		imstb_textedit.h
		imstb_truetype.h
	COPTS
		${CPP_FLAGS}
	DEPS
		freetype
)