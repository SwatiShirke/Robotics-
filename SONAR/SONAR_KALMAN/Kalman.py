class Kalman:
    def __init__(self, window_size,gain,process_gain):
        self.Mean = 0
        self.Varriance = 0
        self.EstValue = 0
        self.EstVarriance = 0
        self.EstValueNew = 0
        self.EstVarrianceNew = 0
        self.K = gain
        self.WindowSize = window_size
        self.Flag = 0
	self.Process_gain = process_gain
	

    def Kalman_continue(self, value_list = []):
         
        if len(value_list) != self.WindowSize :
           print ("Size of the list is not same as that of defined window size of the filter")
           exit()
        
                
        #find mean and varriance
        sum = 0
        for i in range(0,self.WindowSize):
            sum = sum + value_list[i]

        self.Mean = float (sum )/ self.WindowSize
        #print self.Mean
        
        sum = 0
        for i in range (0,self.WindowSize):
            sum = sum + pow(value_list[i] - self.Mean,2)

        self.Varriance = float (sum )/ self.WindowSize 
        ##print self.Varriance
 
        
       	if self.Flag ==0:
           self.EstValueNew = self.Mean
           self.EstVarrianceNew  = self.Varriance
           self.Flag = 1
           ##print "I am here" 
           
        else:
           #predict 
           self.EstValue = self.EstValueNew
           self.EstVarriance = self.EstVarrianceNew  + self.Process_gain

            
	   #print "self.EstVarriance", self.EstVarriance
	   #print "self.Varriance ", self.Varriance
            

           #print "Hi................."
	   #Update sHI
           self.K = self.EstVarriance / (self.EstVarriance + self.Varriance)
           self.EstValueNew = self.EstValue + self.K * float (self.Mean  - self.EstValue)
           self.EstVarrianceNew = self.EstVarriance * float (1- self.K)
        
        ## update state
        
        #print self.EstValue
	return [self.EstValueNew, self.Mean]

        
