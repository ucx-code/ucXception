# pull the official base image
FROM node:alpine
# set working direction
WORKDIR /app
# add `/app/node_modules/.bin` to $PATH
ENV PATH /app/node_modules/.bin:$PATH
# copy files
COPY . ./
# install application dependencies
RUN npm install
# expose port 
EXPOSE 3000
# start app
CMD ["npm", "start"]