
var app = angular.module('alg2', ['PointyPony', 'ui.router']);

app.constant('appConfig', {
  courseToken: "eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.IldFQlMgdGVzdCI.LmMGE0RDudXtO47AmsW3OPhT9aXc0mdBX5JhC7Bj1IQ",
});

app.config(function($stateProvider, $httpProvider) {

    $stateProvider
    .state('home', {
        url: '/',
        templateUrl: 'home.html'
    })
    .state('page', {
        url: '/{url:.+}',
        template: '<div ng-include="url"></div>',
        controller: function($scope, $stateParams) {
            $scope.url = $stateParams.url;
        }
    });
});