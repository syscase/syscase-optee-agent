FROM archlinux/base

RUN curl \
      -s "https://www.archlinux.org/mirrorlist/?country=DE&country=US&protocol=https&use_mirror_status=on" \
      | sed -e 's/^#Server/Server/' -e '/^#/d' > /etc/pacman.d/mirrorlist
RUN pacman -Syyuu --noconfirm
RUN pacman --noconfirm -S cmake clang make gcc python wget

RUN mkdir /syscase
COPY bin /syscase/bin
COPY src /syscase/src
COPY include /syscase/include
COPY vendor /syscase/vendor
COPY CMakeLists.txt /syscase/CMakeLists.txt 
COPY .clang-format /syscase/.clang-format

WORKDIR /syscase
RUN bin/ci.sh
