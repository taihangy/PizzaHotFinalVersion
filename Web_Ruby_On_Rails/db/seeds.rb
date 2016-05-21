#---
# Excerpted from "Agile Web Development with Rails",
# published by The Pragmatic Bookshelf.
# Copyrights apply to this code. It may not be used to create training material, 
# courses, books, articles, and the like. Contact us if you are in doubt.
# We make no guarantees that this code is fit for any purpose. 
# Visit http://www.pragmaticprogrammer.com/titles/rails4 for more book information.
#---
# encoding: utf-8
Product.delete_all
Product.create!(title: 'Pizza1',
  description: 
    %{<p>
        Pizza1 is great.
      </p>},
  image_url:   'dish/dish12.jpg',  
  catagory: "Pizza", 
  price: 6.00)
# . . .
Product.create!(title: 'Pizza2',
  description:
    %{<p>
        Pizza1 is great.
      </p>},
  image_url: 'dish/dish12.jpg',
  catagory: "Pizza", 
  price: 9.95)
# . . .

Product.create!(title: 'Burger1',
  description: 
    %{<p>
        Burge1 is great.
      </p>},
  image_url: 'dish/dish11.jpg',
  catagory: "Burge", 
  price: 4.95)

Product.create!(title: 'Burger2',
  description: 
    %{<p>
        Burge2 is great.
      </p>},
  image_url: 'dish/dish11.jpg',
  catagory: "Burge", 
  price: 5.95)

Product.create!(title: 'Dessert1',
  description: 
    %{<p>
        Dessert1 is great.
      </p>},
  image_url: 'dish/dish14.jpg',
  catagory: "Dessert", 
  price: 2.95)

Product.create!(title: 'Dessert2',
  description: 
    %{<p>
        Dessert2 is great.
      </p>},
  image_url: 'dish/dish16.jpg',
  catagory: "Dessert", 
  price: 3.95)