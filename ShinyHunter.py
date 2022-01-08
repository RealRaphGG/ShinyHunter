'''
Author : RaphGG
'''

# OpenCV
import cv2

# numpy
import numpy as np

# pour créer les fichiers où seront stockées les images
import os

# pour communiquer avec l'arduino
import serial

# numéro port webcam
WEBCAM_NUMBER = 0

# coordonnées du pixel à tester
# voir dans photoshop (fenêtre info)
TEST_PIXEL_X = 200 # horizontal
TEST_PIXEL_Y = 170 # vertical

ARDUINO_PORT = 'COM3'
ARDUINO_BAUD_RATE = 9600

# numéro tentative
ATTEMPT_NUMBER = 0

'''
# defini la plage HSV de la couleur du bouton Attaque (l'animation du shiny retarde l'apparition de l'IU)
# pour connaitre le HSV utiliser le scrypt find_HSV
lower_red = np.array([160, 140, 125]) 
upper_red = np.array([180, 255, 255])

On préferera utiliser le bouton equipe car trop de nuance de rouge dans l'arrière plan, possibilité d'échec
'''

# defini la plage HSV de la couleur du bouton Equipe (l'animation du shiny retarde l'apparition de l'IU)
# pour connaitre le HSV utiliser le scrypt find_HSV
lower_green = np.array([32, 10, 111]) 
upper_green = np.array([103, 183, 239])



#--------------------------------------- Fonction de détection shiny -------------------------------------------

def isItShiny():

    global ATTEMPT_NUMBER 
    ATTEMPT_NUMBER += 1
    print('Tentative n°' + str(ATTEMPT_NUMBER) + '\n')

    # connection à un périphérique (dans ce cas, la webcam)
    cap = cv2.VideoCapture(WEBCAM_NUMBER, cv2.CAP_DSHOW)
    print('Webcam allumé')

    # obtenir une image de la webcam
    ret, frame = cap.read()
    print('Photo prise')

    cap.release()
    print('Webcam éteinte\n')

    '''
    # Pas nécéssaire pour mon cas. Si votre webcam sature parfois (couleur de prixel très variable), activer le en enlevant les 3 apostrophe en haut et en bas de ce commentaire

    # flou gaussien
    frame = cv2.GaussianBlur(frame, (11,11), 0)
    '''

    # conversion RGB -> HSV
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    print('Conversion HSV effectué')


    # masque pour obtenir une image noir et blanc (blanc si la couleur est compris dans l'intervalle, noir si non compris dans l'intervalle)
    mask = cv2.inRange(hsv, lower_green, upper_green)
    print('Masque créé')

    # applique le masque à notre image "frame"
    res = cv2.bitwise_and(frame,frame, mask= mask)
    print('fusion du masque avec la photo effectuée')

    # colore le pixel testé en jaune sur la photo avant de sauvegarder l'image afin de savoir lequel est testé.
    # Cela permettra d'effectuer des réglages de position du pixel de test si necéssaire.
    frame[TEST_PIXEL_Y, TEST_PIXEL_X][2] = 255
    frame[TEST_PIXEL_Y, TEST_PIXEL_X][1] = 255
    frame[TEST_PIXEL_Y, TEST_PIXEL_X][0] = 0
    print('Impression du pixel de test en JAUNE sur la photo ajouté')

    chemin = 'img/' + str(ATTEMPT_NUMBER)

    # on vérifie si le dossier où stocker les images existe, si non, on le créé
    if not os.path.exists(chemin):

        os.makedirs(chemin)

    # enregistrement, str(ATTEMPT_NUMBER) convertit le int ATTEMPT_NUMBER en string
    cv2.imwrite(chemin + '/' + 'img_' + str(ATTEMPT_NUMBER) + '.png', frame) # photo
    cv2.imwrite(chemin + '/' + 'mask_' + str(ATTEMPT_NUMBER) + '.png', mask) # masque
    cv2.imwrite(chemin + '/' + 'res_' + str(ATTEMPT_NUMBER) + '.png', res) # combinaison

    print('Enregistrement des images effectué')

    # si le pixel du masque est blanc (que l'on voit IU), le pokémon n'est pas shiny
    if mask[TEST_PIXEL_Y,TEST_PIXEL_X] == 255:
        return False
    else:
        return True

#--------------------------------------- main -------------------------------------------

print('\nScript lancé\n')


if __name__ == '__main__':

    ser = serial.Serial(ARDUINO_PORT, ARDUINO_BAUD_RATE)

    while True:
        # Empty line
        print()

        # Wait for message from Arduino
        # message = b'Command: checkIfShiny\r\n'
        message = ser.readline()
        print("Message from Arduino: ")
        print(message)

        # Quand le message envoyé par l'arduino est "Command: checkIfShiny", on vérifie si le pokémon est shiny, sinon on ne fait rien (on attend le message)
        if message == b'Command: checkIfShiny\r\n':
            if isItShiny():
                print('Shiny apres ' + str(ATTEMPT_NUMBER) + ' !')
                ser.write(b'y')
            else:
                print('Pas Shiny, je soft reset a ta place t\'inquietes !')
                ser.write(b'n')
        else:
            '''
            Ne fait rien
            '''
