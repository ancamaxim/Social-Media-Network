# Overview
An implementation of a simplified social media platform that uses graph and tree data structures to manage social networks, post interactions, and user feeds. 
This project aims to build a social media platform, combining cloud storage, networking, and social features in one. It integrates various functionalities in three stages, such as friend networks, post creation, reposting, and a social feed.
# Features
## Task 1: Friend Network
- ### Add friend:
    Establish a bi-directional connection between two users.
- ### Remove friend:
    Break the connection between two users.
- ### Distance:
    Calculate the shortest path (friendship chain) between two users.
- ### Suggestions:
    Provide friend suggestions based on mutual connections.
- ### Common friends:
    Show mutual friends between two users.
- ### Popular:
    Find the most popular friend (one with the most connections).

## Task 2: Posts and Reposts

- ### Create post:
    Users can create posts with titles and unique IDs.
- ### Repost:
    Users can repost content from others, which can also be reposted further.
- ### Like:
    Users can like or dislike posts and reposts.
- ### Ratio:
    Determine if a repost has more likes than the original post (ratio).
- ### Delete Post/Repost:
    Delete posts or reposts with cascading effects.
- ### Get-likes:
    Display the number of likes for a post/repost.
- ### Get-reposts:
    Show the entire repost chain for a post.

## Task 3: Social Media Feed

- ### Feed:
    Display the latest posts of a user and their friends.
- ### View profile:
    Display all posts and reposts of a user.
- ### Friends that reposted:
    List friends who reposted a given post.
- ### Common group:
    Find the largest group of friends who are all interconnected.
