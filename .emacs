(setq c-mode-hook
  (function
    (lambda()
     (setq comment-column 40)
     (setq tab-width 4))))
(setq c-tab-always-indent nil)
(setq c-argdecl-indent -5)
(setq c-label-offset 0)
(setq c-continued-statement-offset 0)
(define-key global-map "\C-xl" 'compile)
(define-key global-map "\M-O-C" 'forward-char)
(define-key global-map "\M- " 'set-mark-command)
(define-key global-map "\M-s" 'search-forward)
(define-key global-map "\M-o" 'open-line)
(define-key global-map "\C-x4r" 'query-replace-regexp)
(define-key global-map "\C-xq" 'keyboard-quit)


(autoload 'send-pr "send-pr" "Submit a Problem Report." t)

(autoload 'edit-pr "gnats"
  	  "Command to edit a problem report." t)

(autoload 'view-pr "gnats"
  	  "Command to view a problem report." t)

(autoload 'gnats-mode "gnats"
	  "Major mode for editing of problem reports." t)

(autoload 'query-pr "gnats"
  	  "Command to query information about problem reports." t)

(defvar ee-map (make-sparse-keymap) "Keymap for ESC-ESC")
(define-key esc-map "\e" ee-map)

(define-key ee-map "r" 'query-replace-regexp)
(define-key ee-map "f" 'find-file)
(define-key ee-map "1" 'delete-other-windows)
(define-key ee-map "2" 'split-window-vertically)
(define-key ee-map "b" 'switch-to-buffer)
(define-key ee-map "s" 'save-some-buffers)
(define-key ee-map "c" 'save-buffers-kill-emacs)
(define-key ee-map "k" 'kill-buffer)
(define-key ee-map "l" 'compile)
(define-key ee-map "`" 'next-error)
(define-key ee-map "a" 'beginning-of-line)
(define-key ee-map "e" 'end-of-line)
(define-key ee-map "u" 'universal-argument)
(define-key ee-map "i" 'isearch-repeat-forward)
(define-key global-map "\e\er" 'redraw-display)
(define-key global-map "\e\ed" 'backward-kill-word)
(define-key global-map "\e\en" 'kt-line-numbers-display)
;; k y c

(defun kt-function-header ()
  "Place a function header block starting on next line after current one."
  (interactive)
  (end-of-line)
  (insert "\n/* ")
  (insert-char ?. 72)
  (insert " *\n")
  (insert "| NAME\n")
  (insert "| function - purpose\n")
  (insert "| \n")
  (insert "| DESCRIPTION\n")
  (insert "| As yet undocumented.\n")
  (insert "| \n")
  (insert "| RETURN VALUES\n")
  (insert "| Function return value will be DT_RC_GOOD on success.\n")  
  (insert "| Any other return value indicates failure or noncompletion.\n")
  (insert "| When DT_RC_GOOD is not returned, any other return values may not be valid.\n")
  (insert "| \n")
  (insert "| ERRORS\n")
  (insert "| As yet undocumented.\n")
  (insert "| \n")
  (insert "| END\n")
  (insert " * ")
  (insert-char ?. 72)
  (insert " */\n")
)

(defun kt-end-comment ()
  "Place a C end-comment mark in the final column.
   Also removes spaces from the end of the line."
  (interactive)
  (save-excursion
    (end-of-line)
    (let ((end (point)))
      (skip-chars-backward " \t")
      (delete-region (point) end)
    )
    (skip-chars-backward "/")
    (skip-chars-backward "*")
    (if (looking-at "[*][/]")
        (indent-to 76)
      (end-of-line)
      (indent-to 76)
      (insert "*/")
    )
  )
)



(defun kt-goto-window-line (arg)
  "Go to absolute window-relative line."
  (interactive "NGo to window line: ")
  (save-restriction
    (widen)
    (goto-char (window-start))
    (forward-line (1- arg))))

(defun kt-line-numbers-display ()
  "Make line number window to left of current window."
  (interactive)
  (let ((original-buffer (buffer-name)))
	(setq window-min-width 4)
	(split-window-horizontally 5)
	(switch-to-buffer "kt-line-numbers-buffer")
	(other-window 1)))

(defun kt-line-numbers-create ()
  "Put line numbers in line number window."
  (interactive)
  (save-window-excursion
  (switch-to-buffer "kt-line-numbers-buffer")
  (insert "01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n"
          "11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n"
          "21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n"
          "31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n"
          "41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n"
          "51\n52\n53\n54\n55\n56\n57\n58\n59\n60\n")
  (goto-char 1)))

(kt-line-numbers-create)

(kt-line-numbers-display)

(message "Init file loaded: /home/dave/.emacs")



; Epoch stuff
; (when (boundp 'epoch::version) 
;  (load "/usr/local/emacs/lisp/thing")
;  (load "/usr/local/emacs/lisp/imouse")
;  (rebind-key "Home" nil "\M-<")
;  (rebind-key "Prior" nil "\M-v")
;  (rebind-key "End" nil "\M->")
;  (rebind-key "Next" nil "\C-v")
;  (rebind-key "KP_5" nil "\M-r")
;  (rebind-key "BackSpace" nil "\C-d")
;  (rebind-key "Cancel" nil "\C-g")
;  (rebind-key "Undo" nil "\C-xu")
;  (rebind-key "F11" nil "\C-a")
;  (rebind-key "F12" nil "\C-e")
;  (rebind-key "F8" nil "\C-x\C-f")
;  (rebind-key "F7" nil "\C-xb")
;  (rebind-key "F9" nil "\C-s")
;  (rebind-key "F2" nil "\C-xl")
;  (rebind-key "F5" nil "\C-x4r")
;  (rebind-key "F3" nil "\C-y")
;  (rebind-key "F4" nil "\C-xs")
;  (rebind-key "F1" nil "\C-k")
; )
; Emacs 19.28 for X
   (define-key global-map [f11] 'beginning-of-line)
   (define-key global-map [f12] 'end-of-line)
   (define-key global-map [f8] 'find-file)
   (define-key global-map [f7] 'switch-to-buffer)
   (define-key global-map [f9] 'isearch-forward)
   (define-key global-map [f2] 'compile)
   (define-key global-map [f5] 'query-replace-regexp)
   (define-key global-map [f3] 'yank)
   (define-key global-map [f4] 'save-some-buffers)
   (define-key global-map [f1] 'kill-line)
   (define-key global-map [cancel] 'keyboard-quit)
   (define-key global-map [delete] 'backward-delete-char-untabify)
   (define-key global-map [backspace] 'delete-char)

   (define-key minibuffer-local-map [cancel] 'abort-recursive-edit)
   (define-key minibuffer-local-ns-map [cancel] 'abort-recursive-edit)
   (define-key minibuffer-local-completion-map [cancel] 'abort-recursive-edit)
   (define-key minibuffer-local-must-match-map [cancel] 'abort-recursive-edit)
   (define-key minibuffer-local-isearch-map [cancel] 'abort-recursive-edit)
   (define-key isearch-mode-map [cancel] 'isearch-abort)

;   (define-key minibuffer-local-map "\C-xg" 'abort-recursive-edit)
;   (define-key minibuffer-local-ns-map "\C-xg" 'abort-recursive-edit)
;   (define-key minibuffer-local-completion-map "\C-xg" 'abort-recursive-edit)
;   (define-key minibuffer-local-must-match-map "\C-xg" 'abort-recursive-edit)
;   (define-key minibuffer-local-isearch-map "\C-xg" 'abort-recursive-edit)
;   (define-key isearch-mode-map "\C-xg" 'isearch-abort)

   (define-key isearch-mode-map [f9] 'isearch-repeat-forward)


































