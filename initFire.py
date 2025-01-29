import firebase_admin
from firebase_admin import credentials, db

# Initialize Firebase Admin SDK
cred = credentials.Certificate('key.json')  # Replace with your key
firebase_admin.initialize_app(cred, {
    'databaseURL': 'https://swiftsneak-1379-default-rtdb.europe-west1.firebasedatabase.app/'  # Replace with your database URL 
    #https://swiftsneak-1379-default-rtdb.europe-west1.firebasedatabase.app

})

# Reference the 'distances' node in the RTDB
distances_ref = db.reference('distances')

# Get the current data from RTDB
distances_data = distances_ref.get()
# about:blank#blocked
if distances_data is None:
    distances_data = []

def dist_0(dot:tuple):
    return (dot[0]**2 + dot[1]**2)**0.5

def dist_line(line:tuple, angle:float):
    dot1 = line[0]; dot2 = line[1]
    return abs((dot2[1] - dot1[1])*dot1[0] - (dot2[0] - dot1[0])*dot1[1] + dot2[0]*dot1[1] - dot2[1]*dot1[0])/(dist_0(dot2,dot1)*abs(angle))
    
    
# print("Current distances:", distances_data)
if __name__ == "__main__":
    x = {} 

    for i in range(0, 180):
        j=0
        for j in range(0, 180):
            x[i*180+j] = i%j


    # Update RTDB with the new entries
    updates = x
    distances_ref.update(updates)

    print("Updated distances with:", updates)