; sbcl --script gencode.lisp > key.txt

; This program generates the key to be used for encryption/decryption
; in the Arduino program.

; this must match KEYDIM in the Arduino code
(defvar *keylength* 65535)

(setf *random-state* (make-random-state t))

; the below is 63 because we have here 63 translations
(defun match-suggestion (suggestion)
  (cond ((eq suggestion 0) 'A)
        ((eq suggestion 1) 'B)
        ((eq suggestion 2) 'C)
        ((eq suggestion 3) 'D)
        ((eq suggestion 4) 'E)
        ((eq suggestion 5) 'F)
        ((eq suggestion 6) 'G)
        ((eq suggestion 7) 'H)
        ((eq suggestion 8) 'I)
        ((eq suggestion 9) 'J)
        ((eq suggestion 10) 'K)
        ((eq suggestion 11) 'L)
        ((eq suggestion 12) 'M)
        ((eq suggestion 13) 'N)
        ((eq suggestion 14) 'O)
        ((eq suggestion 15) 'P)
        ((eq suggestion 16) 'Q)
        ((eq suggestion 17) 'R)
        ((eq suggestion 18) 'S)
        ((eq suggestion 19) 'T)
        ((eq suggestion 20) 'U)
        ((eq suggestion 21) 'V)
        ((eq suggestion 22) 'W)
        ((eq suggestion 23) 'X)
        ((eq suggestion 24) 'Y)
        ((eq suggestion 25) 'Z)
        ((eq suggestion 26) '1)
        ((eq suggestion 27) '2)
        ((eq suggestion 28) '3)
        ((eq suggestion 29) '4)
        ((eq suggestion 30) '5)
        ((eq suggestion 31) '6)
        ((eq suggestion 32) '7)
        ((eq suggestion 33) '8)
        ((eq suggestion 34) '9)
        ((eq suggestion 35) '0)
        ((eq suggestion 36) '&)
        ((eq suggestion 37) '+)
        ((eq suggestion 38) '%)
        ((eq suggestion 39) '*)
        ((eq suggestion 40) '?)
        ((eq suggestion 41) '!)

        ((eq suggestion 42) #\~)
        ((eq suggestion 43) #\))
        ((eq suggestion 44) #\()
        ((eq suggestion 45) #\[)
        ((eq suggestion 46) #\])
        ((eq suggestion 47) #\^)
        ((eq suggestion 48) #\$)
        ((eq suggestion 49) #\|)
        ((eq suggestion 50) #\.)
        ((eq suggestion 51) #\,)
        ((eq suggestion 52) #\:)
        ((eq suggestion 53) #\;)
        ((eq suggestion 54) #\=)
        ((eq suggestion 55) #\/)
        ((eq suggestion 56) #\{) ; enter
        ((eq suggestion 57) #\}) ; backspace
        ((eq suggestion 58) #\`)
        ((eq suggestion 59) #\@)
        ((eq suggestion 60) #\")

        (t '_)))

; difficult candidates: space, single-quote, backslash, degree > 127

(defun give-alphabet ()
(let ((alphabet '()) (result '()) (suggestion 0) (len 0))
(loop
  (setf suggestion (list (random 62) (random 62) (random 62) (random 62) (random 62)))
  (cond ((not (member suggestion result :test 'equal))
          (progn (incf len)
          (push suggestion result)
          (push (mapcar 'match-suggestion (cons (random 62) suggestion)) alphabet))))
  (cond ((eq len *keylength*) (return alphabet))))))
; len should not be more than 43*43*43*43*43 = 147,008,443 62^5 = 916,132,832
; (give-alphabet)
; ((9 U ! E D V) (Q 6 E D F O) (L X I Y _ 5) (& N S J 6 *) (I 1 8 E 3 U)
;  (G M R & A Q) (R + Q 7 I E) (1 F E Q _ Y) (+ * Y 8 O !) (I O 5 U 2 0)
;  (T T Z T & &) (M Q + I E L) (R ! U C T 1) (T 9 8 4 W !) (V O 6 + M C)
;  (1 I 0 H R V) (Z 8 G 9 * 6) (5 3 S 9 K 4) (S * L L M 2) (Z S 4 3 * K)
;  (M L 4 + K 1) (J 3 J 9 + P) (9 P S % 0 5) (Q 0 R N * %) (L Q O ! Z O)...)



(defun print-for-c (code-list)
(let ((pretty-count 3) (blk '()))
(progn
(terpri)
(princ "const PROGMEM char keyarray[KEYDIM][6] = ")
(terpri)
(princ #\{)
(loop
  (cond ((null code-list) (return '())))
  (setq blk (pop code-list))

  (progn
   (princ #\{)
   (princ #\')
   (princ (nth 0 blk))
   (princ #\')
   (princ #\,)
   (princ #\')
   (princ (nth 1 blk))
   (princ #\')
   (princ #\,)
   (princ #\')
   (princ (nth 2 blk))
   (princ #\')
   (princ #\,)
   (princ #\')
   (princ (nth 3 blk))
   (princ #\')
   (princ #\,)
   (princ #\')
   (princ (nth 4 blk))
   (princ #\')
   (princ #\,)
   (princ #\')
   (princ (nth 5 blk))
   (princ #\')
   (princ #\}))

  (cond ((not (null code-list)) (princ #\,)))
  (decf pretty-count)
  (cond ((zerop pretty-count) (progn (setq pretty-count 3) (terpri)))))
(princ #\})
(princ #\;)
(terpri)
(terpri)
(finish-output nil)
)))

(print-for-c (give-alphabet))



