import speech_recognition
from jarvis_voices import *

init_()
recognizer = speech_recognition.Recognizer()

while True:
  try:
    with speech_recognition.Microphone() as mic:
      print("Adjusting for ambient noise...")
      recognizer.adjust_for_ambient_noise(mic, duration=0.2)
      print("Listening...")
      audio = recognizer.listen(mic, timeout=None, phrase_time_limit=None)

    text = recognizer.recognize_google(audio_data=audio)
    text = text.lower()
    print("Recognized:", text)

    # speak_text(f"you said {text}")
    if "jarvis"  in text:  
        execute_commands(text)    
    else:
        speak_text(text)
        speak_text("Note:Say JARVIS to perform any JARVIS related tasks")
        

  except speech_recognition.WaitTimeoutError:
    print("Listening timed out, retrying...")
    continue

  except speech_recognition.UnknownValueError:
    print("Could not understand audio, trying again...")
    continue

  except speech_recognition.RequestError as e:
    print(f"API request error: {e}")
    continue

  except KeyboardInterrupt:
    print("Stopping program.")
    break

  except Exception as e:
    print(f"An unexpected error occurred: {e}")
    continue
