filetype off
set shellslash
set exrc
set secure
set nocompatible
set shortmess+=A
set renderoptions=type:directx,level:0.75,gamma:1.25,contrast:0.25,geom:1,renmode:5,taamode:1
set t_Co=256


"set encoding=utf-8
set fileencoding=utf-8


" set the runtime path to include Vundle and initialize
set rtp+=$HOME/vimfiles/bundle/Vundle.vim/
call vundle#begin('$HOME/vimfiles/bundle/')

Plugin 'VundleVim/Vundle.vim'

Plugin 'junegunn/seoul256.vim'

Plugin 'ctrlpvim/ctrlp.vim'
"Plugin 'tacahiroy/ctrlp-funky'
Plugin 'cskeeters/vim-smooth-scroll.git'
"Plugin 'skywind3000/asyncrun.vim'
"Plugin 'srcery-colors/srcery-vim.git'
Plugin 'CreaturePhil/vim-handmade-hero.git'
Plugin 'yegappan/findstr.git'
Plugin 'majutsushi/tagbar.git'
Plugin 'simnalamburt/vim-mundo.git'
"Plugin 'ycm-core/YouCompleteMe.git'


" All of your Plugins must be added before the following line
call vundle#end()            " required
filetype plugin indent on    " required
"-------------------------------------------

let mapleader = "-"
let g:AutoPairsShortcutFastWrap=''

nnoremap gd gd<C-o>
nnoremap <leader>i viB=<C-o>
nnoremap <leader>s [[
nnoremap <leader>d ]]
map <leader>b %
nnoremap S <C-w>r
nnoremap <C-c> I//<esc>
nnoremap <C-u> 0wxx
nnoremap <leader>w :e .<cr>
inoremap jk <esc>
vnoremap jk <esc>
onoremap jk <esc>
inoremap <S-j> ()
inoremap <S-k> {}
inoremap <S-l> []
inoremap <leader>s *
inoremap <leader>r &
inoremap <leader>b <bar>
inoremap <leader>t ~
inoremap <leader>c ""
inoremap ö <BS>
inoremap Ö <C-n>
map <space> mf
nnoremap d d`f
nnoremap vv v$
nnoremap z zz
nnoremap <leader>f :Rfindstring /i<cr>
nnoremap <leader>fx :Rfindstring /x<cr>
nnoremap <leader>g :YcmCompleter GoTo<cr>
nnoremap <leader>r :YcmCompleter GoToReferences<cr>
inoremap <leader>v <
inoremap <leader>e >
inoremap <leader>a ->
map <leader>c <esc>

nnoremap <leader>j viw<esc>a)<esc>bi(<esc>lel
nnoremap <leader>k viw<esc>a}<esc>bi{<esc>lel
nnoremap <leader>l viw<esc>a]<esc>bi[<esc>lel

nnoremap <silent> <F5> :!start "..\build\dev.exe" <CR>
nnoremap <esc> <C-w>c
nnoremap <C-h> :nohl<CR>

" window switching
nnoremap s <C-w>w

nnoremap <C-s> :wa<CR>


" Some funky status bar code its seems
" https://stackoverflow.com/questions/9065941/how-can-i-change-vim-status-line-colour
set laststatus=2            " set the bottom status bar


function! ModifiedColor()
    if &mod == 1
        hi statusline guibg=White ctermfg=8 guifg=Red ctermbg=15
    else
        hi statusline guibg=White ctermfg=8 guifg=DarkSlateGray ctermbg=15
    endif
endfunction

au InsertLeave,InsertEnter,BufWritePost   * call ModifiedColor()

" Formats the statusline
set statusline=%f                           " file name
set statusline+=[%{strlen(&fenc)?&fenc:'none'}, "file encoding
set statusline+=%{&ff}] "file format
set statusline+=%y      "filetype
set statusline+=%h      "help file flag
set statusline+=[%{getbufvar(bufnr('%'),'&mod')?'MODIFIED':'SAVED'}]      
"modified flag

set statusline+=%r      "read only flag

set statusline+=\ %=                        " align left
set statusline+=Line:%l/%L[%p%%]            " line X of Y [percent of file]
set statusline+=\ Col:%c                    " current column
set statusline+=\ Buf:%n                    " Buffer number
set statusline+=\ [%b][0x%B]\               " ASCII and byte code under cursor
" default the statusline when entering Vim
hi statusline guibg=White ctermfg=8 guifg=DarkSlateGray ctermbg=15
set expandtab
set number
set fillchars+=vert:\ 
set autoread
set nosol
set hlsearch
" set autochdir
set cindent
set autoindent
set smartindent
set tabstop=4
set softtabstop=4
set shiftwidth=4
set guifont=Consolas:h14:b
set backspace=indent,eol,start
set incsearch
set guioptions-=m
set guioptions-=T
set guioptions-=r
set guioptions-=L
set cmdheight=2
set ignorecase
set smartcase
set noshowmode
set antialias
set switchbuf-=split
set hidden

set maxmempattern=2000000

" deoplete
"let g:deoplete#enable_at_startup = 1

" CtrlP
" Ignore some folders and files for indexing
"let g:ctrlp_custom_ignore = {
  "\ 'dir':  '\.git$\|\.yardoc\|public$|log\|tmp$',
  "\ 'file': '\.so$\|\.dat$|\.DS_Store$\|\.cfg$\|\.txt$'
  "\ }
"
let g:ctrlp_user_command =  
  \ ['.git', 'cd %s && git ls-files -co --exclude-standard']
let g:ctrlp_cache_dir = $HOME . '/.cache/ctrlp'
let g:ctrlp_max_files = 100
let g:ctrlp_max_depth = 30
let g:ctrlp_by_filename = 1
let g:ctrlp_map = '<C-n>'
let g:ctrlp_match_window = 'top,order:btt,min:1,max:100,results:50'
let g:ctrlp_cmd = 'CtrlPLastMode'
let g:ctrlp_extensions = ['buffertag', 'tag', 'line', 'dir']

nnoremap t :CtrlPBuffer<CR>
nnoremap <C-j> :CtrlPCurWD<CR>
nnoremap <C-l> :CtrlPLine<CR>

" vim-plug; for other plugin managers, use what's appropriate
" if you don't want to trust a prebuilt binary, skip the 'do' part
" and build the binaries yourself. Instructions are further down
" and place them in the /path/to/plugin/rplugin/python3 folder

"Plug 'raghur/fruzzy', {'do': { -> fruzzy#install()}}

" optional - but recommended - see below
"let g:fruzzy#usenative = 1

" When there's no input, fruzzy can sort entries based on how similar they are to the current buffer
" For ex: if you're on /path/to/somefile.h, then on opening denite, /path/to/somefile.cpp
" would appear on the top of the list.
" Useful if you're bouncing a lot between similar files.
" To turn off this behavior, set the variable below  to 0

"let g:fruzzy#sortonempty = 1 " default value

" tell CtrlP to use this matcher
"let g:ctrlp_match_func = {'match': 'fruzzy#ctrlp#matcher'}
"let g:ctrlp_match_current_file = 1 " to include current file in matches

" CtrlPFunky
nnoremap <S-f> :CtrlPFunky<CR>
"let g:ctrlp_funky_sort_by_mru = 1
let g:ctrlp_funky_multi_buffers = 1


" AsyncRun
let g:asyncrun_open = 8

" YCM
let g:ycm_seed_identifiers_with_syntax=1
let g:ycm_global_ycm_extra_conf = 'C:\Code\Repos\feature\ui\Engine\.ycm_extra_conf.py'
let g:ycm_confirm_extra_conf=1
let g:ycm_collect_identifiers_from_tag_files = 1
let g:ycm_always_populate_location_list = 1
set completeopt=longest,menu

" Airline
let g:airline_theme = 'minimalist'

" Findstr
let Findstr_Default_Filelist = '*.c *.h *.cpp *.hpp'


" Tagbar
nnoremap <leader>t :TagbarToggle<CR>
let g:tagbar_ctags_bin = 'C:\Ctags\ctags.exe'

nmap <F2> :MundoToggle<CR>


"colorscheme srcery
colorscheme handmade-hero


hi clear SignColumn
syntax on

"caret color stuff
highlight Cursor guifg=white guibg=red
highlight iCursor guifg=white guibg=green
set guicursor=n-v-c:block-Cursor
set guicursor+=i:ver100-iCursor
set guicursor+=a:blinkon0

set cursorline
highlight CursorLine guibg=#001000 guifg=fg
autocmd InsertLeave * highlight CursorLine guibg=#001000 guifg=fg
autocmd InsertEnter * highlight CursorLine guibg=#300000 guifg=fg

autocmd BufNew,BufRead *.asm set ft=masm

hi NonText guifg=bg

" Thanks to https://forums.handmadehero.org/index.php/forum?view=topic&catid=4&id=704#3982
" error message formats
" Microsoft MSBuild
set errorformat+=\\\ %#%f(%l\\\,%c):\ %m
" Microsoft compiler: cl.exe
set errorformat+=\\\ %#%f(%l)\ :\ %#%t%[A-z]%#\ %m
" Microsoft HLSL compiler: fxc.exe
set errorformat+=\\\ %#%f(%l\\\,%c-%*[0-9]):\ %#%t%[A-z]%#\ %m


"vimrc mappings
nnoremap <leader>v :e $MYVIMRC<CR>

function! DoBuildBatchFile()
   :normal :w<CR>
   " build.bat
   set makeprg=build
   " Make sure the output doesnt interfere with anything
   silent make
   " Open the output buffer
   cwindow 5
   echo 'Build Complete'
   ":normal s
endfunction

" Highlight all instances of word under cursor, when idle.
" Useful when studying strange source code.
nnoremap '/ :if AutoHighlightToggle()<Bar>set hls<Bar>endif<CR>
function! AutoHighlightToggle()
  let @/ = ''
  if exists('#auto_highlight')
    au! auto_highlight
    augroup! auto_highlight
    setl updatetime=50
    echo 'Highlight current word: off'
    return 0
  else
    augroup auto_highlight
      au!
      au CursorHold * let @/ = '\V\<'.escape(expand('<cword>'), '\').'\>'
    augroup end
    setl updatetime=50
    echo 'Highlight current word: ON'
    return 1
  endif
endfunction

if !exists(":DiffOrig")
  command DiffOrig vert new | set bt=nofile | r # | 0d_ | diffthis
          \ | wincmd p | diffthis
endif


"Go to previous error
nnoremap <F7> :cp<CR><CR>
"Go to next error
nnoremap <F8> :cn<CR><CR>

nnoremap , :wa<CR> :call DoBuildBatchFile()<CR>

augroup BgHighlight
    autocmd!
    autocmd WinEnter * set cul
    autocmd WinLeave * set nocul
augroup END


" No error sounds
set noerrorbells visualbell t_vb=0
autocmd GUIEnter * set visualbell t_vb=0
au GUIEnter * simalt ~x

