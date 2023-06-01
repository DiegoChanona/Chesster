import serial 

puerto = 'COM4'
velocidad = 9600
ser = serial.Serial(puerto, velocidad)
datos = 'Hola Arduino!\n'  # Reemplaza con los datos que deseas enviar
ser.write(datos.encode())
ser.close() # Se cierrra la conexión serie al terminar de mandar los datos 
