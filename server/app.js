const express = require('express')

const app = express();


app.use((req, res, next)=>{
    console.log('something happened')
    next()
})

app.get('/', (req, res)=>{
    console.log('/ route hit')
    res.sendFile('/home.js')
})
app.get('/about', (req, res)=>{
    console.log('/about route hit')
    res.sendFile('/about.js')
})

const port = 80
app.listen(port, function(){
    console.log(`listen on ${port}`)
    console.log("print")
});
