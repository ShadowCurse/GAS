create table DeveloperRole
(
    id   int generated always as identity primary key not null,
    role text                                         not null
);

create table DeveloperInfo
(
    id   int generated always as identity primary key not null,
    name text                                         not null,
    age  int                                          not null,
    role int                                          not null,
    foreign key (role) references DeveloperRole (id)
);

create table Project
(
    id          int generated always as identity primary key not null,
    name        text                                         not null,
    description text
);

create table Developer
(
    id       int generated always as identity primary key not null,
    dev_info int                                          not null,
    project  int                                          not null,
    foreign key (dev_info) references DeveloperInfo (id),
    foreign key (project) references Project (id)
);

create table ResourceType
(
    id   int generated always as identity primary key not null,
    name text                                         not null
);

create table Resource
(
    id          int generated always as identity primary key not null,
    name        text                                         not null,
    description text,
    size        int                                          not null,
    checksum    uuid                                         not null,
    type        int                                          not null,
    data        date                                          not null,
    foreign key (type) references ResourceType (id)
);
drop table Resource;

create table Dependency
(
    id                  int generated always as identity primary key not null,
    requesting_resource int                                          not null,
    required_resource   int                                          not null,
    foreign key (requesting_resource) references Resource (id),
    foreign key (required_resource) references Resource (id)
);
drop table Dependency;

create table Commits
(
    id       int generated always as identity primary key not null,
    dev      int                                          not null,
    resource int                                          not null,
    date     timestamp                                    not null,
    message  text,
    foreign key (dev) references Developer (id),
    foreign key (resource) references Resource (id)
);
drop table Commits;

-- create user admin with

insert into Project(name, description) values ('PogProject', 'kekW');

insert into DeveloperRole(role) values ('CharacterDesigner');
insert into DeveloperRole(role) values ('Animator');
insert into DeveloperRole(role) values ('LevelDesigner');

insert into DeveloperInfo(name, age, role) values ('Dev1', 20, 1);
insert into DeveloperInfo(name, age, role) values ('Dev2', 30, 1);
insert into DeveloperInfo(name, age, role) values ('Dev3', 40, 3);

insert into Developer(dev_info, project) values ((select id from DeveloperInfo where name = 'Dev1'), (select id from Project where name = 'PogProject'));
insert into Developer(dev_info, project) values ((select id from DeveloperInfo where name = 'Dev2'), (select id from Project where name = 'PogProject'));
insert into Developer(dev_info, project) values ((select id from DeveloperInfo where name = 'Dev3'), (select id from Project where name = 'PogProject'));

insert into ResourceType(name) values ('Texture');
insert into ResourceType(name) values ('Material');
insert into ResourceType(name) values ('Sprite');
insert into ResourceType(name) values ('Mesh');
insert into ResourceType(name) values ('Level');

insert into Resource(name, description, size, checksum, type, data) values ('Level1', '', 12, 'A0EEBC99-9C0B-4EF8-BB6D-6BB9BD380A11', 5, '12-1-2000');
insert into Resource(name, description, size, checksum, type, data) values ('Material1', '', 12, 'A0EAAC89-9C0B-4EF8-BB6D-6BB9BD380A11', 2, '12/2/2010');
