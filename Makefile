#CFLAGS += -g -DNDEBUG -Wall -pedantic -pipe $(INCLUDES)
#LDFLAGS += -g -Wall -pipe
INCLUDES=-I/opt/local/include/GraphicsMagick -I.
IMAGEMAGICK = -L/opt/local/lib -lGraphicsMagick++
CFLAGS += -g -Wall -pipe $(INCLUDES)
LDFLAGS += -g -Wall -pipe $(IMAGEMAGICK) -framework OpenGL -framework GLUT


TARGET = texbind
# C++ Files
CXXFILES = texbind.cpp Texture.cpp msgfxerror.cpp SurfaceGeometry.cpp misc.cpp
CFILES =

# Headers
HEADERS = Texture.h msgfxerror.h GLSLShader.h SurfaceGeometry.h SurfaceGeometryFaceList.h misc.h

OBJECTS = $(CXXFILES:.cpp=.o) $(CFILES:.c=.o)

DEP = $(CXXFILES:.cpp=.d) $(CFILES:.c=.d)

default all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJECTS) $(LLDLIBS)

-include $(DEP)

%.d: %.cpp
	set -e; $(CXX) -MM $(CFLAGS) $< \
	| sed 's/($*).o[ :]*/.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@

%.d: %.c
	set -e; $(CXX) -MM $(CFLAGS) $< \
	| sed 's/($*).o[ :]*/.o $@ : /g' > $@; \
	[ -s $@ ] || rm -f $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

%.o: %.c
	$(CXX) $(CFLAGS) -c $<

clean:
	-rm -f $(OBJECTS) core $(TARGET).core *~

spotless: clean
	-rm -f $(TARGET) $(DEP)

