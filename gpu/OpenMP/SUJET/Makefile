TARGET=tp1

all: $(TARGET) $(TARGET)_correction

$(TARGET):
	pdflatex --jobname=$(TARGET) $(TARGET) 
	pdflatex --jobname=$(TARGET) $(TARGET) 

$(TARGET)_correction:
	pdflatex --jobname=$(TARGET)_correction "\def\tpcorrection{YES} \include{$(TARGET)}"
	pdflatex --jobname=$(TARGET)_correction "\def\tpcorrection{YES} \include{$(TARGET)}"

clean: clean_tmp
	rm -f *.pdf

clean_tmp:
	rm -f *.aux *.log *.out

