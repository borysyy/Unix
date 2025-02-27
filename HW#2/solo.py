# CS 528 - Unix Network Programming
# Program: client.c
# Assignment: HW#2
# Authors: Slava Borysyuk, Taylor Hunter
# Date: 02/09/2025
# This program is a client that connects to two different AI models and sends messages to them. 
# The messages are then printed out by the server.

# Import the Client class from the ollama module
from ollama import Client

# Create a client object
client = Client(
    host="http://localhost:11435",  # The host 
    headers={
        'Content-Type': 'application/json' # The headers 
    }
)
# Create a new user chat history 
chat_history = []

# Send a message to the server
while(1):
    user_input = str(input("Message: ")) # Get the user input
    chat_history.append({"role": "user", "content": user_input}) # Add the user input to the chat history

    response = client.chat(  # Call the chat method
        model="llama3.1",    # The model
        messages=chat_history  # The chat history
    )

    chat_history.append(response['message'])  # Append the response to the chat history
    ai_response = response['message']['content']  # Get the content of the response
    
    print(f"AI Response {ai_response}")  # Print the AI response

