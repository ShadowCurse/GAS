                                                                     create table DeveloperRole
(
    id   int generated always as identity primary key not null,
    role text                                         not null
);

create table DeveloperInfo
(
    id   int        generated always as identity primary key not null,
    name text                                                not null,
    age  int                                                 not null,
    role int                                                 not null,
    foreign key (role) references DeveloperRole (id) on delete cascade
);

create table Project
(
    id          int generated always as identity primary key not null,
    name        text                                         not null,
    description text
);

create table Developer
(
    id       int         generated always as identity primary key not null,
    dev_info int                                                  not null,
    project  int                                                  not null,
    foreign key (dev_info) references DeveloperInfo (id) on delete cascade,
    foreign key (project) references Project (id)        on delete cascade
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
    data        oid                                          not null,
    foreign key (type) references ResourceType (id)  on delete cascade
);

create table Dependency
(
    id                  int     generated always as identity primary key not null,
    requesting_resource int                                              not null,
    required_resource   int                                              not null,
    foreign key (requesting_resource) references Resource (id)  on delete cascade,
    foreign key (required_resource) references Resource (id)    on delete cascade
);

create table Commits
(
    id       int   generated always as identity primary key not null,
    dev      int                                            not null,
    resource int                                            not null,
    date     timestamp                                      not null,
    message  text,
    foreign key (dev) references Developer (id)    on delete cascade,
    foreign key (resource) references Resource (id) on delete cascade
);