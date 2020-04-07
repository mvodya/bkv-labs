#!/bin/bash

# Operation systems
# Lab 5 - Universal unarchiver
# by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, Danil Maltsev

function unpack_dir_part {
  mkdir "$1"
  mv "$2" "$1"
  cd "$1"
  eval $3
  if [ $? -eq 0 ]
  then
    rm "$2";
    unpack_dir
  fi
  cd ..
}

function unpack_dir {
  for i in *
  do
    if [ -d "$i" ]
    then
      cd -- "$i"
      unpack_dir
      cd ..
    fi
    if [ -f "$i" ]
    then
      case $i in
        ######################################################
        *.[Zz][Ii][Pp])
          file=$i
          dir=`basename "${file%.[Zz][Ii][Pp]}"`
          unpack_dir_part "$dir" "$i" "unzip -P x \"$i\" &>/dev/null"
          ;;

        ######################################################
        *.t7z | *.tar.7z | *.txz)
          case $i in
            *.t7z)
              dir=`basename "$i" .t7z` ;;
            *.tar.7z)
              dir=`basename "$i" .tar.7z` ;;
            *.txz)
              dir=`basename "$i" .txz` ;;
          esac
          unpack_dir_part "$dir" "$i" "tar xJf \"$i\" &>/dev/null"
          ;;

        ######################################################
        *.tbz | *.tbz2 | *.tar.bz2 | *.tar.bz)
          case $i in
            *.tbz2)
              dir=`basename "$i" .tbz2` ;;
            *.tbz)
              dir=`basename "$i" .tbz` ;;
            *.tar.bz2)
              dir=`basename "$i" .tar.bz2` ;;
            *.tar.bz)
              dir=`basename "$i" .tar.bz` ;;
          esac
          unpack_dir_part "$dir" "$i" "tar xjf \"$i\" &>/dev/null"
          ;;

        ######################################################
        *.7z | *.xz)
          case $i in
            *.7z)
              dir=`basename "$i" .7z` ;;
            *.xz)
              dir=`basename "$i" .xz` ;;
          esac
          unpack_dir_part "$dir" "$i" "7z x \"$i\" &>/dev/null"
          ;;

        ######################################################
        *.bz2)
          dir=`basename "$i" .bz2` 
          unpack_dir_part "$dir" "$i" "bzip2 -dk \"$i\" &>/dev/null"
          ;;

        #####################################################
        *.gz)
          dir=`basename "$i" .gz`
          unpack_dir_part "$dir" "$i" "gzip -k -d \"$i\" &>/dev/null"
          ;;

        #####################################################
        *.tar.gz | *.tgz)
          case $i in
            *.tar.gz)
              dir=`basename "$i" .tar.gz` ;;
            *.tgz)
              dir=`basename "$i" .tgz` ;;
          esac
          unpack_dir_part "$dir" "$i" "tar xzvf \"$i\" &>/dev/null"
          ;;

        #####################################################
        *.tar)
          dir=`basename "$i" .tar`
          unpack_dir_part "$dir" "$i" "tar xvf \"$i\" &>/dev/null"
          ;;

        #####################################################
        *.rar)
          dir=`basename "$i" .rar`
          unpack_dir_part "$dir" "$i" "unrar x \"$i\" &>/dev/null"
          ;;  
      esac
    fi
  done
}

unpack_dir