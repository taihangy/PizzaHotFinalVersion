'use strict';

angular.module('conFusion.services', ['ngResource'])
  .constant("baseURL","http://10.132.3.54:3000/")

  .factory('$localStorage', ['$window', function($window) {
    return {
      store: function(key, value) {
        $window.localStorage[key] = value;
      },
      get: function(key, defaultValue) {
        return $window.localStorage[key] || defaultValue;
      },
      storeObject: function(key, value) {
        $window.localStorage[key] = JSON.stringify(value);
      },
      getObject: function(key, defaultValue) {
        return JSON.parse($window.localStorage[key] ||defaultValue);
      }
    }
  }])

  .factory('menuFactory', ['$resource', 'baseURL', function($resource, baseURL) {
      return $resource(baseURL+"dishes/:id", null,  {'update':{method:'PUT' }});
  }])

  .factory('promotionFactory', ['$resource', 'baseURL', function($resource, baseURL) {
      return $resource(baseURL+"promotions/:id");
  }])

  .factory('corporateFactory', ['$resource', 'baseURL', function($resource,baseURL) {

      return $resource(baseURL+"leadership/:id");

  }])

  .factory('feedbackFactory', ['$resource', 'baseURL', function($resource,baseURL) {

      return $resource(baseURL+"feedback/:id");

  }])

  .factory('favoriteFactory', ['$resource', 'baseURL', '$localStorage', function($resource, baseURL, $localStorage) {
      var favFac = {};
      var favorites = $localStorage.getObject('userfavorites', '[]');

      favFac.addToFavorites = function (index) {
        var exsit = false;
        for (var i = 0; i < favorites.length; i++) {
          if (favorites[i].id === index) {
            exsit = true;
          }
        }
        if (!exsit) {
          favorites.push({id:index});
        }
        $localStorage.storeObject('userfavorites', favorites);
      };

      favFac.getFavorites = function() {
        return favorites;
      };

      favFac.deleteFromFavorites = function (index) {
        for (var i = 0; i < favorites.length; i++) {
          if (favorites[i].id === index) {
            favorites.splice(i, 1);
            break;
          }
        }
        $localStorage.storeObject('userfavorites', favorites);
      };
      
      return favFac;
  }])

;