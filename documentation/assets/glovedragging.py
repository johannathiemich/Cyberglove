def controlHandleDrag():
    import VR
   
    # must run on loop trigger
   
    if not hasattr(VR, 'isDragging'):
        VR.isDragging = False
       
    glove = VR.find('vrpn_device')
    if not glove: return # in testing enviroments, dont run the script
   
    b = VR.find('ART_tracker.2')
    b.findAll('av_ray')[0].show()  
   
    def startDrag():
        if glove.intersect(VR.getRoot()):
            VR.isDragging = True
            print "Attempting to start dragging, found intersecting object(s)."
            obj = glove.getIntersected()
            print "Intersecting Object:" + str(obj)
            glove.drag(obj)
        else:
            print "Attempting to start dragging, but no intersecting object."
       
    def stopDrag():
        VR.isDragging = False
        glove.drop()
   
    if hasattr(glove, 'getSlider') and hasattr(glove, 'setBeacon'):
        num = glove.getSlider(100) # bend sensor data
        glove.setBeacon(VR.find('ART_tracker.2'))
    else:
        num = 0
   
   
    # If gripping while not dragging the handle, start dragging it.
    if num >= 10 and not VR.isDragging:
        VR.fGlove = 0
        VR.touchedHandle = True
        startDrag()
       
    # If loosening the grip while dragging the handle, stop dragging it.
    # Also stio dragging if a challenge is running
    if (num < 10 and VR.isDragging) or VR.isChallenge:
        VR.fGlove = 1
        stopDrag()