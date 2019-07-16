if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
imap <End> g$
imap <Home> g^
map Q gq
vmap gx <Plug>NetrwBrowseXVis
nmap gx <Plug>NetrwBrowseX
noremap h i
noremap i k
noremap j h
noremap k j
vnoremap <silent> <Plug>NetrwBrowseXVis :call netrw#BrowseXVis()
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#BrowseX(netrw#GX(),netrw#CheckIfRemote(netrw#GX()))
map <End> g$
map <Home> g^
cmap bt !ctags -h=.c.h.cpp -R
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set autoread
set background=dark
set backspace=indent,eol,start
set expandtab
set fileencodings=ucs-bom,utf-8,default,latin1
set helplang=en
set hlsearch
set incsearch
set modelines=0
set mouse=a
set ruler
set runtimepath=~/.vim,~/.vim/bundle/dart-vim-plugin,~/.vim/bundle/typescript-vim,~/.vim/bundle/vim-coffee-script,~/.vim/bundle/vim-go,~/.vim/bundle/vim-javascript,~/.vim/bundle/vim-jsx,~/.vim/bundle/vim-less,~/.vim/bundle/vim-markdown,/usr/share/vim/vimfiles,/usr/share/vim/vim80,/usr/share/vim/vimfiles/after,~/.vim/bundle/vim-markdown/after,~/.vim/bundle/vim-less/after,~/.vim/bundle/vim-jsx/after,~/.vim/bundle/vim-coffee-script/after,~/.vim/after,/usr/share/vim/addons,/usr/local/go/misc/vim,~/tagbar
set shiftwidth=4
set showcmd
set smarttab
set noswapfile
set tabstop=4
set window=0
" vim: set ft=vim :
