import pyttsx3
import pygame
import os
import requests

ESP32_IP = "192.168.29.88"

initialization_audio_path = r"D:\AI-Robotics-Projects\Open Legged Robotics Platform\JARVIS - Open Legged Robotic Platform\Open Legged Robotic Platform\Audio Files\welcome_back_jarvis.mp3"
def init_(audio_path=initialization_audio_path):
    pygame.mixer.init()
    try:
        if os.path.exists(audio_path):
            pygame.mixer.music.load(audio_path)
            pygame.mixer.music.play()
            while pygame.mixer.music.get_busy():
                pygame.time.Clock().tick(10)
        else:
            print(f"Audio file not found: {audio_path}")
    except Exception as ex:
        print(f"Audio Playback Error: {ex}")
        speak_text("error in initializing jarvis")

def speak_text(text):
  try:
    engine = pyttsx3.init()
    voices = engine.getProperty("voices")
    engine.setProperty("voice", voices[0].id)
    engine.say(text)
    engine.runAndWait()
    engine.stop()
  except Exception as ex:
    print(f"TTS Error: {ex}")

def execute_commands(text):
    if "music please" in text:
        speak_text("Playing ROCK Songs")
        print("Playing some rock music") 
    elif (("rest" in text) or ("sit" in text)):
        send_command("rest")
        speak_text("Sitting Down")
    elif (("stand" in text) or ("up" in text)):
        send_command("stand")
        speak_text("Standing Up")
    elif (("hello" in text) or ("hii" in text)):
        send_command("hello")
        speak_text("Hello Sir")


def play_songs():
    print("playing songs")


def send_command(command: str):
    url = f"http://{ESP32_IP}/command"
    try:
        response = requests.get(
            url,
            params={"cmd": command},
            timeout=2
        )
        if response.status_code == 200:
            message = response.text
            print("JARVIS:", message)
            speak_text(message)
        else:
            print("ESP32 error:", response.text)
    except requests.RequestException as e:
        print("Could not reach JARVIS:", e)